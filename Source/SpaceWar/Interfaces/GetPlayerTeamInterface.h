// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpaceWar/Enums/PlayerTeamEnum.h"
#include "GetPlayerTeamInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGetPlayerTeamInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPACEWAR_API IGetPlayerTeamInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ETeam FindPlayerTeam();
};
