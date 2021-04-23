// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponObject.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "Net/UnrealNetwork.h"

UBaseWeaponObject::UBaseWeaponObject()
{
	
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

void UBaseWeaponObject::SetCharacterOwner(ASpaceWarCharacter* NewOwner)
{
	CharacterOwner = NewOwner;
	OnRep_CharOwner();
}

void UBaseWeaponObject::OnRep_CharOwner()
{
	OnOwnerChanged.Broadcast();
}





