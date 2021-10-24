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
	DOREPLIFETIME(UBaseWeaponObject, WeaponName);
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
	return !GetWorld()->GetTimerManager().IsTimerActive(UseWeaponHandle);
}

bool UBaseWeaponObject::UseWeapon()
{
	if(IsAbleToUseWeapon())
	{
		if(CharacterOwner->Controller)
		{
			bWeaponUsed = true;
			CharacterOwner->SetCanWeaponManipulation(false);
		}
		return true;
	}
	StopUseWeapon();
	return false;
}

void UBaseWeaponObject::StopUseWeapon()
{
	if(!bWeaponUsed) return;
	OnWeaponUsed.Broadcast(false);
	bWeaponUsed = false;
	if(CharacterOwner->Controller)
	{
		CharacterOwner->SetCanWeaponManipulation(true);
		if(CharacterOwner->IsLocallyControlled())
		{
			CharacterOwner->StopRecoilTimeline();
		}
	}
}

void UBaseWeaponObject::Server_StartUseWeapon_Implementation()
{
	if(IsAbleToUseWeapon() && CharacterOwner->IsCanWeaponManipulation())
	{
		UseWeapon();
		NetMulticast_StartUseWeapon();

		if(CharacterOwner->GetNetMode() != ENetMode::NM_DedicatedServer)
		{
			OnWeaponUsed.Broadcast(true);
		}
	}
}

void UBaseWeaponObject::Server_StopUseWeapon_Implementation()
{
	StopUseWeapon();
	NetMulticast_StopUseWeapon();
}

bool UBaseWeaponObject::OwnerStartUseWeapon()
{
	if(IsAbleToUseWeapon() && CharacterOwner->IsCanWeaponManipulation())
	{
		UseWeapon();
		Server_StartUseWeapon();
		OnWeaponUsed.Broadcast(true);
		return true;
	}
	return false;
}

void UBaseWeaponObject::OwnerStopUseWeapon()
{
	if(bWeaponUsed)
	{
		StopUseWeapon();
		Server_StopUseWeapon();
	}
}

void UBaseWeaponObject::NetMulticast_StartUseWeapon_Implementation()
{
	if(!CharacterOwner->Controller)
	{
		OnWeaponUsed.Broadcast(true);
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