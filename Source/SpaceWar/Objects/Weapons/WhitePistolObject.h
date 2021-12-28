// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/RangeWeaponObjectBase.h"
#include "WhitePistolObject.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UWhitePistolObject : public URangeWeaponObjectBase
{
	GENERATED_BODY()
	
public:

	UWhitePistolObject();

protected:

	virtual bool OwnerStartUseWeapon() override;
	virtual bool UseWeapon() override ;
	virtual void Server_StartUseWeapon() override;
	virtual void StartDelayBeforeUsedTimer() override;
	virtual float GetDelayBeforeUsed() const override { return bPrimaryFiringMode ? Super::GetDelayBeforeUsed() : Super::GetDelayBeforeUsed() * 6.f; }
	virtual float CalculateBaseDamage() const override { return bPrimaryFiringMode ? Super::CalculateBaseDamage() : Super::CalculateBaseDamage() * 1.25f; } 
	virtual FText CurrentFiringModeText() const override { return bPrimaryFiringMode ? FText::FromString("Single") : FText::FromString("Triple"); }

private:

	int32 CurrentAmountFiring;
};
