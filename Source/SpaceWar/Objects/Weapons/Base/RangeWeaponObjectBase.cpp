// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponObjectBase.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/Interfaces/DropHelperTraceInterface.h"
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
    	CurrentAmmoInWeapon = WeaponData.AmmoStatistics.MaxAmmoInWeapon;
    	CurrentAmmoInStorage = WeaponData.AmmoStatistics.MaxAmmoInStorage;
		CurrentSpread = WeaponData.Spreads.MinSpread;
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
		auto const AttachToComponent = CharacterOwner->GetWeaponMesh();
		FVector Location;
		FRotator Rotation;
		CharacterOwner->GetWeaponMesh()->GetSocketWorldLocationAndRotation("Muzzle", Location, Rotation);
		
		UGameplayStatics::SpawnEmitterAttached(WeaponData.Particles.MuzzleParticle, AttachToComponent, 
			"Muzzle", Location, Rotation, FVector(0.6f), EAttachLocation::KeepWorldPosition);

		if(CharacterOwner->Controller)
		{
			PlaySound2DByCue(WeaponData.SoundData.FireMuzzleSound);
		}
		else
		{
			PlaySoundByCue(WeaponData.SoundData.FireMuzzleSound, AttachToComponent, "Muzzle");
		}
	}
}

bool URangeWeaponObjectBase::UseWeapon()
{
	if(!Super::UseWeapon()) return false;
	
	if(CharacterOwner->Controller)
	{
		CurrentAmmoInWeapon--;
	}

	GetWorld()->GetTimerManager().SetTimer(UseWeaponHandle, this, &URangeWeaponObjectBase::StopRateDelay,
		WeaponData.WeaponCharacteristicsBase.DelayBeforeUse, false);

	DropLineTrace();

	PlayUseWeaponEffects();
	
	return true;
}

void URangeWeaponObjectBase::StopRateDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(UseWeaponHandle);

	if(WeaponData.RangeWeaponCharacteristics.bCanAutoFire && bWeaponUsed)
	{
		UseWeapon();
	}
}

void URangeWeaponObjectBase::StopUseWeapon()
{
	Super::StopUseWeapon();

	CurrentSpread = GetWeaponData().Spreads.MinSpread;
}

bool URangeWeaponObjectBase::IsAbleToUseWeapon()
{
	return Super::IsAbleToUseWeapon() && (!CharacterOwner->Controller || CurrentAmmoInWeapon > 0) && !bReloading;
}

FVector URangeWeaponObjectBase::FindRotateAround()
{
	FVector RotateAroundVector = CharacterOwner->GetActorForwardVector().RotateAngleAxis(CharacterOwner->GetLookUpPitch(),CharacterOwner->GetActorRightVector());
 	RotateAroundVector.Z *= -1;
	return RotateAroundVector;
}

FVector URangeWeaponObjectBase::GetShootDirection()
{
	FVector const RotateAroundVector = FindRotateAround();
	CurrentSpread += WeaponData.Spreads.MaxSpread / WeaponData.AmmoStatistics.MaxAmmoInWeapon;
	float const TempSpread = bAccessoryUsed ? ((10.f / FMath::Max(1, WeaponData.Spreads.AccuracyInSight) - 1) * 0.5f) : CurrentSpread;
	
	/** Rotate trace with horizontal */
	FVector const HorizontalRotate = FindRotateAround().RotateAngleAxis(UKismetMathLibrary::RandomFloatInRangeFromStream(TempSpread * -1,
		TempSpread, WeaponData.Spreads.FireRandomStream), FRotationMatrix(RotateAroundVector.Rotation()).GetScaledAxis(EAxis::Y));

	/** reottae trace with use up vector */
	FVector const VerticalRotate = HorizontalRotate.RotateAngleAxis(UKismetMathLibrary::RandomFloatInRangeFromStream(TempSpread * -1,
		TempSpread, WeaponData.Spreads.FireRandomStream), FRotationMatrix(RotateAroundVector.Rotation()).GetScaledAxis(EAxis::Z));

	return VerticalRotate;
}

void URangeWeaponObjectBase::DropLineTrace()
{
	FVector const TraceStart = CharacterOwner->GetCurrentFireTrace();
	FVector const TraceEnd = WeaponData.RangeWeaponCharacteristics.RangeOfUse * GetShootDirection() + TraceStart;

	FHitResult Hit;
                                                                                                  
	FCollisionQueryParams QueryParams;                                                                
	QueryParams.AddIgnoredActor(CharacterOwner);                                                      
                                                                                                  
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_GameTraceChannel4, QueryParams);
                                                                                                  
	DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, IsAuthority() ? FColor::Purple : FColor::Green, false, 0.5f);
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 14.f, 6,FColor::Yellow, false, 0.5f);

	if(IsAuthority())
	{
		ApplyPointDamage(Hit);
	}
}

void URangeWeaponObjectBase::ApplyPointDamage(const FHitResult& Hit)
{
	if(Hit.GetActor())
	{
		FVector const HitFromDirection = UKismetMathLibrary::GetDirectionUnitVector(Hit.TraceEnd, Hit.TraceStart);
		int32 const TempDistance = FVector::Distance(CharacterOwner->GetActorLocation(), Hit.Actor->GetActorLocation()) - WeaponData.RangeWeaponCharacteristics.MaxDamageDistance;

		float const TempDamage = TempDistance > WeaponData.RangeWeaponCharacteristics.MaxDamageDistance ? FMath::Max(WeaponData.RangeWeaponCharacteristics.BaseDamage - TempDistance / 100,
			WeaponData.RangeWeaponCharacteristics.BaseDamage / 1.5f) : WeaponData.RangeWeaponCharacteristics.BaseDamage;
		
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), TempDamage, HitFromDirection, Hit, CharacterOwner->Controller,
			CharacterOwner, UDamageType::StaticClass());
	}
}

bool URangeWeaponObjectBase::IsAbleToReload()
{
	return CurrentAmmoInWeapon < WeaponData.AmmoStatistics.MaxAmmoInWeapon && CurrentAmmoInStorage > 0 && CharacterOwner->IsCanWeaponManipulation();
}

void URangeWeaponObjectBase::OnRep_Reload()
{
	if(CharacterOwner->Controller)
	{
		PlaySound2DByCue(WeaponData.SoundData.ReloadSound);
	}
	else
	{
		PlaySoundByCue(WeaponData.SoundData.ReloadSound, CharacterOwner->GetWeaponMesh()->GetComponentLocation(), FRotator::ZeroRotator);
	}
	
	OnReload.Broadcast(bReloading);
}

void URangeWeaponObjectBase::ReloadStart()
{
	if(IsAbleToReload())
	{
		bReloading = true;
		OnRep_Reload();
		CharacterOwner->SetCanWeaponManipulation(false);
		GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &URangeWeaponObjectBase::ReloadStop, WeaponData.RangeWeaponCharacteristics.ReloadTime, false);
	}
}

void URangeWeaponObjectBase::ReloadStop()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
	bReloading = false;
	OnRep_Reload();
	CharacterOwner->SetCanWeaponManipulation(true);

	/** calculate need ammo */
	int32 const NeedToMaxAmmo = WeaponData.AmmoStatistics.MaxAmmoInWeapon - CurrentAmmoInWeapon;
		
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
	if(bReloading || CharacterOwner->GetStaminaComponent()->IsStaminaUse()) return;
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

void URangeWeaponObjectBase::AddAmmo(int32 const Amount)
{
	if(CurrentAmmoInStorage >= WeaponData.AmmoStatistics.MaxAmmoInStorage) return;
	
	CurrentAmmoInStorage = FMath::Min(WeaponData.AmmoStatistics.MaxAmmoInStorage, CurrentAmmoInStorage + Amount);
}

FString URangeWeaponObjectBase::GetAmmoStatus() const
{
	return FString::FromInt(CurrentAmmoInWeapon) + " | " + FString::FromInt(CurrentAmmoInStorage);
}

void URangeWeaponObjectBase::PlaySoundByCue(USoundCue* Sound, const FVector& Location, const FRotator& Rotation)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location, Rotation, Sound->VolumeMultiplier,
		Sound->PitchMultiplier, 0, Sound->AttenuationSettings);
}

void URangeWeaponObjectBase::PlaySoundByCue(USoundCue* Sound, USkeletalMeshComponent* AttachComponent, const FName& SocketName)
{
	FVector const  Location = AttachComponent->GetSocketLocation("SocketName");
	bool const bStoWhenAttachedToDestroyed = false;
	UGameplayStatics::SpawnSoundAttached(Sound, AttachComponent, SocketName, Location, FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget, bStoWhenAttachedToDestroyed, Sound->VolumeMultiplier, Sound->PitchMultiplier, 0, Sound->AttenuationSettings);
}

void URangeWeaponObjectBase::PlaySound2DByCue(USoundCue* Sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), Sound, Sound->VolumeMultiplier, Sound->PitchMultiplier, 0, nullptr, nullptr, false);
}

bool URangeWeaponObjectBase::OwnerStartUseWeapon()
{
	if(!Super::OwnerStartUseWeapon())
	{
		if(CurrentAmmoInWeapon <= 0 && CharacterOwner->IsCanWeaponManipulation()) PlaySound2DByCue(WeaponData.SoundData.BlankShot);
		return false;
	}
	return true;
}

