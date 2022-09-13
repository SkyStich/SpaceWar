// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceWar/Components/GameMode/HubServerDataBaseComponent.h"
#include "MainHUBGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API AMainHUBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AMainHUBGameModeBase();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ServerDeactivate();

	UFUNCTION()
	virtual void ManuallyShuttingDownServer();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UHubServerDataBaseComponent* HubDataBaseComponent;
};
