// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseArmorObject.h"
#include "Engine/DemoNetDriver.h"
#include "SpaceWar/SpaceWarCharacter.h"

void UBaseArmorObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UBaseArmorObject, ArmorData, COND_OwnerOnly);
}

int32 UBaseArmorObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	Super::GetFunctionCallspace(Function, Stack);
	return GetOuter() ? GetOuter()->GetFunctionCallspace(Function, Stack) : FunctionCallspace::Local;
}

bool UBaseArmorObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	Super::CallRemoteFunction(Function, Parms, OutParms, Stack);
	if(!GetOuter()) return false;

	UNetDriver* NetDriver = Cast<AActor>(GetOuter())->GetNetDriver();

	if(!NetDriver) return false;

	NetDriver->ProcessRemoteFunction(Cast<AActor>(GetOuter()), Function, Parms, OutParms, Stack, this);
	return true;
}