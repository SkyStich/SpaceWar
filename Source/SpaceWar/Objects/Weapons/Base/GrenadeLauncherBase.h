// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangeWeaponObjectBase.h"
#include "SpaceWar/Actors/Match/Projectile/GrenadeRifle/GrenadeToTouchProjectile.h"
#include "GrenadeLauncherBase.generated.h"

UCLASS()
class SPACEWAR_API UGrenadeLauncherBase : public URangeWeaponObjectBase
{
	GENERATED_BODY()

public:

	UGrenadeLauncherBase();

protected:

	virtual void ShotLogic() override;

private:

	TSubclassOf<AGrenadeToTouchProjectile> SecondaryProjectileClass;
};
