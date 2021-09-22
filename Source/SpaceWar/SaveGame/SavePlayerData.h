// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SpaceWar/Components/EquipableWeaponManager.h"
#include "SpaceWar/GameInstances/BaseGameInstance.h"
#include "SavePlayerData.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API USavePlayerData : public USaveGame
{
	GENERATED_BODY()

public:

	USavePlayerData();

	UPROPERTY()
	TMap<EWeaponType, FName> WeaponData;

	UPROPERTY()
	FName EquippableArmorId;
};
