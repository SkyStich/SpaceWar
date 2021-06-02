// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"
#include "UObject/Interface.h"
#include "FirstAidInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFirstAidInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPACEWAR_API IFirstAidInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartPlayerFirstAid(ETeam Team, float const Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopPlayerFirstAid();
};
