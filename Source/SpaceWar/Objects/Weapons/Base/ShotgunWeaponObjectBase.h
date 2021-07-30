// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangeWeaponObjectBase.h"
#include "ShotgunWeaponObjectBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UShotgunWeaponObjectBase : public URangeWeaponObjectBase
{
	GENERATED_BODY()

protected:

	virtual void DropLineTrace() override;
};
