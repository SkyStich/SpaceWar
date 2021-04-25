// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponObject.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "Net/UnrealNetwork.h"

UBaseWeaponObject::UBaseWeaponObject()
{
	bWeaponUsed = false;
}

int32 UBaseWeaponObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	return CharacterOwner ? CharacterOwner->GetFunctionCallspace(Function, Stack) : FunctionCallspace::Local;
}

bool UBaseWeaponObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	if(!CharacterOwner) return false;

	UNetDriver* NetDriver = CharacterOwner->GetNetDriver();

	if(!NetDriver) return false;

	NetDriver->ProcessRemoteFunction(CharacterOwner, Function, Parms, OutParms, Stack, this);
	return true;
}

void UBaseWeaponObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseWeaponObject, CharacterOwner);
	DOREPLIFETIME(UBaseWeaponObject, WeaponData);
	DOREPLIFETIME_CONDITION(UBaseWeaponObject, bWeaponUsed, COND_SkipOwner);
}

void UBaseWeaponObject::PostInitProperties()
{
	Super::PostInitProperties();

	if(GetOuter() && GetOuter()->GetWorld()) BeginPlay();
}

void UBaseWeaponObject::BeginPlay()
{
	if(IsAuthority())
	{
		SetCharacterOwner(Cast<ASpaceWarCharacter>(GetOuter()));
	}
}

void UBaseWeaponObject::OnWeaponSelectingEvent(bool NewState)
{
	StopUseWeapon();
}

void UBaseWeaponObject::Init(const FEquipWeaponData& NewData)
{
	WeaponData = NewData;
}

void UBaseWeaponObject::SetCharacterOwner(ASpaceWarCharacter* NewOwner)
{
	CharacterOwner = NewOwner;
	OnRep_CharOwner();
}

void UBaseWeaponObject::OnRep_CharOwner()
{
	OnOwnerChanged.Broadcast();
	CharacterOwner->GetWeaponManager()->OnWeaponSelect.AddDynamic(this, &UBaseWeaponObject::OnWeaponSelectingEvent);
}

bool UBaseWeaponObject::IsAbleToUseWeapon()
{
	return !GetWorld()->GetTimerManager().IsTimerActive(UseWeaponHandle) && !CharacterOwner->GetWeaponManager()->GetWeaponSelect();
}

bool UBaseWeaponObject::UseWeapon()
{
	if(!IsAbleToUseWeapon()) return false;

	GetWorld()->GetTimerManager().SetTimer(UseWeaponHandle, this, &UBaseWeaponObject::StopRateDelay, WeaponData.DelayBeforeUse, false);

	return true;
}

void UBaseWeaponObject::StopRateDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(UseWeaponHandle);

	if(WeaponData.bCanAutoFire && bWeaponUsed)
	{
		UseWeapon();
	}
}

void UBaseWeaponObject::StopUseWeapon()
{
	bWeaponUsed = false;
}

void UBaseWeaponObject::Server_StartUseWeapon_Implementation()
{
	UseWeapon();
	NetMulticast_StartUseWeapon();
}

void UBaseWeaponObject::Server_StopUseWeapon_Implementation()
{
	if(bWeaponUsed)
	{
		StopUseWeapon();
		NetMulticast_StopUseWeapon();
	}
}

void UBaseWeaponObject::OwnerStartUseWeapon()
{
	if(IsAbleToUseWeapon())
	{
		UseWeapon();
		Server_StartUseWeapon();
	}
}

void UBaseWeaponObject::OwnerStopUseWeapon()
{
	StopUseWeapon();
	Server_StopUseWeapon();
}

void UBaseWeaponObject::NetMulticast_StartUseWeapon_Implementation()
{
	if(!CharacterOwner->Controller)
	{
		UseWeapon();
	}
}

void UBaseWeaponObject::NetMulticast_StopUseWeapon_Implementation()
{
	if(!CharacterOwner->Controller)
	{
		StopUseWeapon();
	}
}
