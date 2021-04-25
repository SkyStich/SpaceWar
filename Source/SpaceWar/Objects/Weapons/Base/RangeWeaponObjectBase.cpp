// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponObjectBase.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
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
	Super::Init(NewData);
	
	if(IsAuthority())
    {
    	CurrentAmmoInWeapon = WeaponData.MaxAmmoInWeapon;
    	CurrentAmmoInStorage = WeaponData.MaxAmmoInStorage;
    }
}

void URangeWeaponObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(URangeWeaponObjectBase, CurrentAmmoInStorage, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(URangeWeaponObjectBase, CurrentAmmoInWeapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(URangeWeaponObjectBase, bReloading, COND_SkipOwner);
}

void URangeWeaponObjectBase::OnWeaponSelectingEvent(bool NewState)
{
	Super::OnWeaponSelectingEvent(NewState);

	ClearReload();
}

bool URangeWeaponObjectBase::UseWeapon()
{
	if(!Super::UseWeapon()) return false;
	
	if(CharacterOwner->Controller)
	{
		bWeaponUsed = true;
		CurrentAmmoInWeapon--;
	}

	FHitResult OutHit;
	DropLineTrace(OutHit);

	if(IsAuthority())
	{
		ApplyPointDamage();
	}
	
	return true;
}

void URangeWeaponObjectBase::StopUseWeapon()
{
	Super::StopUseWeapon();
}

bool URangeWeaponObjectBase::IsAbleToUseWeapon()
{
	return Super::IsAbleToUseWeapon() && (!CharacterOwner->Controller || CurrentAmmoInWeapon > 0);
}

FVector URangeWeaponObjectBase::GetShootDirection()
{
	FVector RotateAroundVector = CharacterOwner->GetActorForwardVector().RotateAngleAxis(CharacterOwner->GetLookUpPitch(), CharacterOwner->GetActorRightVector());
	RotateAroundVector.Z *= -1;
	return RotateAroundVector;
}

void URangeWeaponObjectBase::DropLineTrace(FHitResult& Hit)
{
	FVector const TraceStart = CharacterOwner->GetFollowCamera()->GetComponentLocation();             
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

void URangeWeaponObjectBase::ApplyPointDamage()
{

}

bool URangeWeaponObjectBase::IsAbleToReload()
{
	return !bWeaponUsed && CurrentAmmoInWeapon < WeaponData.MaxAmmoInWeapon && CurrentAmmoInStorage > 0;
}

void URangeWeaponObjectBase::OnRep_Reload()
{
	OnReload.Broadcast(bReloading);
}

void URangeWeaponObjectBase::ReloadStart()
{
	if(IsAbleToReload())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("ReloadStart"));
		bReloading = true;
		OnRep_Reload();
		GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &URangeWeaponObjectBase::ReloadStop, WeaponData.ReloadTime, false);
	}
}

void URangeWeaponObjectBase::ReloadStop()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
	bReloading = false;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("ReloadStop"));
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

