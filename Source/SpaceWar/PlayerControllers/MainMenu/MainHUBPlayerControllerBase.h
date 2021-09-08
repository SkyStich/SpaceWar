// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuPlayerControllerBase.h"
#include "SpaceWar/Structs/MapInfo.h"

#include "MainHUBPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AMainHUBPlayerControllerBase : public AMainMenuPlayerControllerBase
{
	GENERATED_BODY()
		
	UFUNCTION(Server, Reliable)
    void Server_CreateServer(const FString& MapReference, const FString& ServerName);
	
	UFUNCTION()
	void CheckingForExistenceServer(FString ServerName);

	UFUNCTION()
	void OnCreteServerResult(bool bResult, const FString& Address);

public:

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
    void CreateServer(const FMapData& Data, FString ServerName);

private:

	bool bRequestSent;
	FMapData CreateData;
	FCreateServerCompelete OnCreateServerComplete;
};
