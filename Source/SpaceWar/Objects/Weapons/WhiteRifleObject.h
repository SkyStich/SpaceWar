// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/RangeWeaponObjectBase.h"
#include "WhiteRifleObject.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UWhiteRifleObject : public URangeWeaponObjectBase
{
	GENERATED_BODY()

protected:

	virtual bool IsAbleToAutoFire() const override { return Super::IsAbleToAutoFire() && bPrimaryFiringMode; }
	virtual bool IsCanAutoFire() const override { return bPrimaryFiringMode; }
	virtual float CalculateBaseDamage() const override { return bPrimaryFiringMode ? Super::CalculateBaseDamage() : Super::CalculateBaseDamage() * 1.25f; } 
	virtual FText CurrentFiringModeText() const override { return bPrimaryFiringMode ? FText::FromString("Auto") : FText::FromString("Single"); }
};
