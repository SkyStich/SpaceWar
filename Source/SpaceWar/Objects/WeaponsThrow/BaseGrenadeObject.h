// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/Objects/WeaponsThrow/ThrowWeaponBase.h"
#include "BaseGrenadeObject.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UBaseGrenadeObject : public UThrowWeaponBase
{
	GENERATED_BODY()

protected:

	void GrenadeThrow(float const TotalTimeBeforeExplosion, bool bThrowSucceeded, AController* Controller);

	virtual bool UseWeapon() override;
	virtual void StopUseWeapon() override;
	virtual void PreparationForUse() override;

private:

	bool bWantPreparation;
	FTimerHandle ExplosionHandle;
};
