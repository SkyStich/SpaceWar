// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "CaptureHoldController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ACaptureHoldController : public AMatchPlayerControllerBase
{
	GENERATED_BODY()

public:

	ACaptureHoldController();
	virtual bool SpawnPlayer(const FVector& Location) override;
	virtual void LaunchRespawnTimer(float const Time);

protected:

	UFUNCTION(Server, Unreliable)
	void Server_SpawnPlayer();

	virtual void SetupInputComponent() override;

protected:

	FTimerHandle RespawnTimer;
	bool bCanSpawn;
};
