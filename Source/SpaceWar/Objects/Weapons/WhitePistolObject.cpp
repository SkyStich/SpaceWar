// Fill out your copyright notice in the Description page of Project Settings.


#include "WhitePistolObject.h"
#include "SpaceWar/SpaceWarCharacter.h"

UWhitePistolObject::UWhitePistolObject()
{
	CurrentAmountFiring = 0;
}

bool UWhitePistolObject::OwnerStartUseWeapon()
{
	if(bPrimaryFiringMode) return Super::OwnerStartUseWeapon();

	if(CurrentAmmoInWeapon >= 3)
	{
		return Super::OwnerStartUseWeapon();
	}
	if(CharacterOwner->IsCanWeaponManipulation()) PlaySound2DByCue(WeaponData.SoundData.BlankShot);
	return false;
}

void UWhitePistolObject::Server_StartUseWeapon()
{
	if(CurrentAmmoInWeapon >= 3 || bPrimaryFiringMode)
	{
		Super::Server_StartUseWeapon();
	}
}

void UWhitePistolObject::StartDelayBeforeUsedTimer()
{
	if(bPrimaryFiringMode || CurrentAmountFiring >= 2)
	{
		Super::StartDelayBeforeUsedTimer();
	}
}

bool UWhitePistolObject::UseWeapon()
{
	if(bPrimaryFiringMode) return Super::UseWeapon();
	
	for(CurrentAmountFiring = 0; CurrentAmountFiring < 3; CurrentAmountFiring++)
	{
		if(!Super::UseWeapon())
		{
			CurrentAmountFiring = 0;
			return false;
		}
	}
	return true;
}

