// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowWeaponBase.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/SpaceWarCharacter.h"

UThrowWeaponBase::UThrowWeaponBase()
{
}

void UThrowWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UThrowWeaponBase, ThrowData);
	DOREPLIFETIME_CONDITION(UThrowWeaponBase, CurrentAmountThrow, COND_OwnerOnly);
}

void UThrowWeaponBase::Init(const FBaseThrowData& NewData)
{
	ThrowData = NewData;
	CurrentAmountThrow = ThrowData.MaxAmount;
}

bool UThrowWeaponBase::IsAbleToUseWeapon()
{
	return Super::IsAbleToUseWeapon() && (!CharacterOwner->Controller || CurrentAmountThrow > 0) && !GetWorld()->GetTimerManager().IsTimerActive(PreparationForUseHandle) && !bWeaponUsed;
}

bool UThrowWeaponBase::UseWeapon()
{
	if(!IsAbleToUseWeapon()) return false;
	
	if(CharacterOwner->Controller)
	{
		CurrentAmountThrow--;
		bWeaponUsed = true;
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UThrowWeaponBase::PreparationForUse);
		GetWorld()->GetTimerManager().SetTimer(PreparationForUseHandle, TimerDel, 1.f, false);
		return true;
	}
	return false;
}

void UThrowWeaponBase::StopUseWeapon()
{
	Super::StopUseWeapon();
}

void UThrowWeaponBase::IncrementCurrentAmount()
{
	if(CurrentAmountThrow >= ThrowData.MaxAmount) return;

	CurrentAmountThrow++;
}





