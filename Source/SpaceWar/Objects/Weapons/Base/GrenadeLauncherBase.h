// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangeWeaponObjectBase.h"
#include "SpaceWar/Actors/Match/Projectile/GrenadeRifle/GrenadeStandartProjectile.h"
#include "SpaceWar/Actors/Match/Projectile/GrenadeRifle/GrenadeToTouchProjectile.h"
#include "GrenadeLauncherBase.generated.h"

UCLASS()
class SPACEWAR_API UGrenadeLauncherBase : public URangeWeaponObjectBase
{
	GENERATED_BODY()

public:

	UGrenadeLauncherBase();

	virtual FText CurrentFiringModeText() const override { return bPrimaryFiringMode ? FText::FromString("Ricocheting") : FText::FromString("Impact"); }

protected:

	virtual void ShotLogic() override;

private:

	TSubclassOf<AGrenadeToTouchProjectile> SecondaryProjectileClass;
	TSubclassOf<AGrenadeStandartProjectile> FirstlyProjectileClass;
};
