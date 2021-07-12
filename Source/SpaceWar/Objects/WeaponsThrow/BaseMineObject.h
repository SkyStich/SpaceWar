// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowWeaponBase.h"
#include "BaseMineObject.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UBaseMineObject : public UThrowWeaponBase
{
	GENERATED_BODY()

protected:

	void FindPositionForSettingsMine(AController* OwnerController);
	virtual void PreparationForUse() override;
	virtual void StopUseWeapon() override;
	virtual bool UseWeapon() override;
	virtual bool IsAbleToUseWeapon() override;
	
	void OuterDead();
	class ABaseMineProjectile* SettingsMine(AController* OwnerController, const FVector& SpawnLocation);
};
