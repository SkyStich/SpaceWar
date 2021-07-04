// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "SpaceWar/PlayerStart/PointCapturePlayerStart.h"
#include "CaptureHoldController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPreparationSpawnPlayer);

UCLASS()
class SPACEWAR_API ACaptureHoldController : public AMatchPlayerControllerBase
{
	GENERATED_BODY()

	UFUNCTION()
	void SpawnPlayerPressed();

	UFUNCTION(Server, Unreliable)
	void Server_SpawnPlayerByPoint(const TArray<APointCapturePlayerStart*>&PointArray);

	UFUNCTION(Client, Unreliable)
	void Client_ErrorMessage(const FString& Message);

	UFUNCTION()
	void MatchEnded(const FString& Reason, ETeam WinnerTeam);
	
public:

	ACaptureHoldController();
	virtual bool SpawnPlayer(const FVector& Location) override;
	virtual void LaunchRespawnTimer(float const Time);

	UFUNCTION(BlueprintCallable)
	void SpawnPlayerByPoint(EPointNumber Point);

protected:

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

public:

	FPreparationSpawnPlayer OnPreparationSpawnPlayer;

protected:

	FTimerHandle RespawnTimer;
	bool bCanSpawn;
};
