// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ConnectToServerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AConnectToServerGameMode : public AGameModeBase
{
	GENERATED_BODY()


protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintPure)
	FString GetAddress();
};
