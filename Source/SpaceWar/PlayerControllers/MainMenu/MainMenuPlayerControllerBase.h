// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceWar/Structs/UserInfo.h"
#include "SpaceWar/Components/PlayerController/Transfer/ClientServerTransfer.h"
#include "SpaceWar/Components/PlayerController/Transfer/DataBaseTransfer.h"
#include "MainMenuPlayerControllerBase.generated.h"

UCLASS()
class SPACEWAR_API AMainMenuPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	AMainMenuPlayerControllerBase();

	UFUNCTION(BlueprintPure)
	UClientServerTransfer* GetClientServerTransfer() const { return ClientServerTransfer; }
	
	UFUNCTION(BlueprintPure)
	UDataBaseTransfer* GetDataBaseTransfer() const { return DataBaseTransfer; }

protected:

	UPROPERTY()
	UDataBaseTransfer* DataBaseTransfer;

	UPROPERTY()
	UClientServerTransfer* ClientServerTransfer;
};
