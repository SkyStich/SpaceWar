// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "SpaceWar/PlayerStart/PointCapturePlayerStart.h"
#include "CaptureHoldController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPreparationSpawnPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeginSpawnPlayer);

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
	
	UFUNCTION()
	void OnPreparationStartGameFinishEvent(bool bResult);
	
public:

	ACaptureHoldController();
	virtual bool SpawnPlayer(const FVector& Location) override;
	virtual void LaunchRespawnTimer();

	UFUNCTION(BlueprintCallable)
	bool SpawnPlayerByPoint(EPointNumber Point);

	UFUNCTION(BlueprintPure)
	bool GetIsCanSpawn() const { return bCanSpawn; }

	UFUNCTION(BlueprintPure)
	float GetRespawnTime() const { return RespawnTime; }

protected:

	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UPROPERTY(BlueprintAssignable)
	FPreparationSpawnPlayer OnPreparationSpawnPlayer;
	
	FBeginSpawnPlayer OnBeginSpawnPlayer;

protected:

	FTimerHandle RespawnTimer;
	
	UPROPERTY(Replicated)
	bool bCanSpawn;

	UPROPERTY(EditDefaultsOnly)
	float RespawnTime;
};
