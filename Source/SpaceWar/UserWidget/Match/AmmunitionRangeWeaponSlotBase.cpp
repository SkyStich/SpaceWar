// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmunitionRangeWeaponSlotBase.h"

void UAmmunitionRangeWeaponSlotBase::Init(EWeaponType NewType, FName NewId)
{
	Type = NewType;
	Id = NewId;
}
