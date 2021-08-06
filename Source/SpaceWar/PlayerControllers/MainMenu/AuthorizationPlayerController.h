// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuPlayerControllerBase.h"
#include "SpaceWar/Components/PlayerController/Transfer/ClientServerTransfer.h"
#include "SpaceWar/Components/PlayerController/Transfer/DataBaseTransfer.h"
#include "AuthorizationPlayerController.generated.h"

UCLASS(Blueprintable)
class SPACEWAR_API AAuthorizationPlayerController : public AMainMenuPlayerControllerBase
{
	GENERATED_BODY()

public:

	AAuthorizationPlayerController();
	
	FORCEINLINE UClientServerTransfer* GetClientServerTransfer() const { return ClientServerTransfer; }
	FORCEINLINE UDataBaseTransfer* GetDataBaseTransfer() const { return DataBaseTransfer; }

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	UClientServerTransfer* ClientServerTransfer;

	UPROPERTY()
	UDataBaseTransfer* DataBaseTransfer; 
};
