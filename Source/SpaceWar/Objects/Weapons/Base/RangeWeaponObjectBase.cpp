// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponObjectBase.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

URangeWeaponObjectBase::URangeWeaponObjectBase() 
{
}

void URangeWeaponObjectBase::BeginPlay()
{
	Super::BeginPlay();	
}

void URangeWeaponObjectBase::Init(const FEquipWeaponData& NewData)
{	
	if(IsAuthority())
    {
		WeaponData = NewData;
    	CurrentAmmoInWeapon = WeaponData.MaxAmmoInWeapon;
    	CurrentAmmoInStorage = WeaponData.MaxAmmoInStorage;
		CurrentSpread = WeaponData.MinSpread;
    }
}

void URangeWeaponObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(URangeWeaponObjectBase, CurrentAmmoInStorage, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(URangeWeaponObjectBase, CurrentAmmoInWeapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(URangeWeaponObjectBase, bReloading, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(URangeWeaponObjectBase, bAccessoryUsed, COND_SkipOwner);
	DOREPLIFETIME(URangeWeaponObjectBase, WeaponData);
}

void URangeWeaponObjectBase::OnWeaponSelectingEvent(bool NewState)
{
	Super::OnWeaponSelectingEvent(NewState);

	ClearReload();
}

void URangeWeaponObjectBase::PlayUseWeaponEffects()
{
	if(CharacterOwner->GetNetMode() != NM_DedicatedServer)
	{
		FVector Location;
		FRotator Rotation;
		CharacterOwner->GetWeaponMesh()->GetSocketWorldLocationAndRotation("Muzzle", Location, Rotation);
		UGameplayStatics::SpawnEmitterAttached(WeaponData.MuzzleParticle, CharacterOwner->GetWeaponMesh(), "Muzzle", Location, Rotation, FVector(1.f), EAttachLocation::KeepWorldPosition);
		UGameplayStatics::SpawnSoundAttached(WeaponData.MuzzleSound, CharacterOwner->GetWeaponMesh(), "Muzzle", Location, Rotation, EAttachLocation::SnapToTarget, false);
	}
}

bool URangeWeaponObjectBase::UseWeapon()
{
	if(!Super::UseWeapon()) return false;
	
	if(CharacterOwner->Controller)
	{
		bWeaponUsed = true;
		CurrentAmmoInWeapon--;
	}

	GetWorld()->GetTimerManager().SetTimer(UseWeaponHandle, this, &URangeWeaponObjectBase::StopRateDelay, WeaponData.DelayBeforeUse, false);

	FHitResult OutHit;
	DropLineTrace(OutHit);

	if(IsAuthority())
	{
		ApplyPointDamage(OutHit);
	}

	PlayUseWeaponEffects();
	
	return true;
}

void URangeWeaponObjectBase::StopRateDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(UseWeaponHandle);

	if(WeaponData.bCanAutoFire && bWeaponUsed)
	{
		UseWeapon();
	}
}

void URangeWeaponObjectBase::StopUseWeapon()
{
	Super::StopUseWeapon();

	CurrentSpread = GetWeaponData().MinSpread;
}

bool URangeWeaponObjectBase::IsAbleToUseWeapon()
{
	return Super::IsAbleToUseWeapon() && (!CharacterOwner->Controller || CurrentAmmoInWeapon > 0) && !bReloading;
}

FVector URangeWeaponObjectBase::GetShootDirection()
{
	FVector RotateAroundVector = CharacterOwner->GetActorForwardVector().RotateAngleAxis(CharacterOwner->GetLookUpPitch(), CharacterOwner->GetActorRightVector());
	RotateAroundVector.Z *= -1;

	CurrentSpread += WeaponData.MaxSpread / WeaponData.MaxAmmoInWeapon;
	float const TempSpread = bAccessoryUsed ? 0.f : CurrentSpread;
	
	/** Rotate trace with horizontal */
	FVector const HorizontalRotate = RotateAroundVector.RotateAngleAxis(UKismetMathLibrary::RandomFloatInRangeFromStream(TempSpread * -1, TempSpread, WeaponData.FireRandomStream), FRotationMatrix(RotateAroundVector.Rotation()).GetScaledAxis(EAxis::Y));

	/** reottae trace with use up vector */
	FVector const VerticalRotate = HorizontalRotate.RotateAngleAxis(UKismetMathLibrary::RandomFloatInRangeFromStream(TempSpread * -1, TempSpread, WeaponData.FireRandomStream), FRotationMatrix(RotateAroundVector.Rotation()).GetScaledAxis(EAxis::Z));

	return VerticalRotate;
}

void URangeWeaponObjectBase::DropLineTrace(FHitResult& Hit)
{
	FVector const TraceStart = CharacterOwner->GetActiveCamera()->GetComponentLocation();             
	FVector const TraceEnd = WeaponData.RangeOfUse * GetShootDirection() + TraceStart;                
                                                                                                  
	FCollisionObjectQueryParams ObjectQueryParams;                                                    
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);   
                                                                                                  
	FCollisionQueryParams QueryParams;                                                                
	QueryParams.AddIgnoredActor(CharacterOwner);                                                      
                                                                                                  
	GetWorld()->LineTraceSingleByObjectType(Hit,TraceStart, TraceEnd, ObjectQueryParams, QueryParams);
                                                                                                  
	DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, FColor::Green, false, 0.5f);
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 14.f, 6,FColor::Yellow, false, 0.5f);                                                      	   
}

void URangeWeaponObjectBase::ApplyPointDamage(const FHitResult& Hit)
{
	if(Hit.GetActor())
	{
		FVector const HitFromDirection = UKismetMathLibrary::GetDirectionUnitVector(Hit.TraceEnd, Hit.TraceStart);
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), WeaponData.BaseDamage, HitFromDirection, Hit, CharacterOwner->Controller, CharacterOwner, UDamageType::StaticClass());
	}
}

bool URangeWeaponObjectBase::IsAbleToReload()
{
	return CurrentAmmoInWeapon < WeaponData.MaxAmmoInWeapon && CurrentAmmoInStorage > 0 && !bReloading;
}

void URangeWeaponObjectBase::OnRep_Reload()
{
	StopUseWeapon();
	OnReload.Broadcast(bReloading);
}

void URangeWeaponObjectBase::ReloadStart()
{
	if(IsAbleToReload())
	{
		bReloading = true;
		OnRep_Reload();
		GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &URangeWeaponObjectBase::ReloadStop, WeaponData.ReloadTime, false);
	}
}

void URangeWeaponObjectBase::ReloadStop()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
	bReloading = false;
	OnRep_Reload();

	if(CharacterOwner->Controller)
	{
		int32 const NeedToMaxAmmo = WeaponData.MaxAmmoInWeapon - CurrentAmmoInWeapon;
		
		if(CurrentAmmoInStorage >= NeedToMaxAmmo)
		{
			CurrentAmmoInWeapon += NeedToMaxAmmo;
			CurrentAmmoInStorage -= NeedToMaxAmmo;
		}
		else
		{
			CurrentAmmoInWeapon += CurrentAmmoInStorage;
			CurrentAmmoInStorage = 0;
		}
	}
}

void URangeWeaponObjectBase::Server_ReloadStart_Implementation()
{
	ReloadStart();
}

void URangeWeaponObjectBase::OwnerReload()
{
	if(IsAbleToReload())
	{
		ReloadStart();
		Server_ReloadStart();
	}
}

void URangeWeaponObjectBase::ClearReload()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
	bReloading = false;
}

bool URangeWeaponObjectBase::UseCurrentPlayerObject_Implementation()
{
	OwnerStartUseWeapon();
	return true;
}

void URangeWeaponObjectBase::UnUseCurrentPlayerObject_Implementation()
{
	OwnerStopUseWeapon();
}

void URangeWeaponObjectBase::StartAdditionalUsed()
{
	if(bReloading) return;
	bAccessoryUsed = true;
	OnRep_AccessoryUse();
}

void URangeWeaponObjectBase::StopAdditionalUsed()
{
	bAccessoryUsed = false;
	OnRep_AccessoryUse();
}

void URangeWeaponObjectBase::OnRep_AccessoryUse()
{
	OnAccessoryUsed.Broadcast(bAccessoryUsed);
	StopUseWeapon();
}

bool URangeWeaponObjectBase::OwnerStartAdditionalUsed()
{
	if(bReloading) return false;
	StartAdditionalUsed();
	return true;
}

bool URangeWeaponObjectBase::OwnerStopAdditionalUsed()
{
	if(!bAccessoryUsed) return false;
	StopAdditionalUsed();
	return true;
}