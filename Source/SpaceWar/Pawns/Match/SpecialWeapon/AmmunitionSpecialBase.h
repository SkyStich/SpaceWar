// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/Actors/Match/SpecialWeapon/SpecialWeaponObjectBase.h"
#include "SpaceWar/Interfaces/CurrentCharacterObjectInterface.h"
#include "AmmunitionSpecialBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AAmmunitionSpecialBase : public ASpecialWeaponObjectBase
{
	GENERATED_BODY()

public:

	bool InteractionObject_Implementation(ASpaceWarCharacter* Player) override;
};
