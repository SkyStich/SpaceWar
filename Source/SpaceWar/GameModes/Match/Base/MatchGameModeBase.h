// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameModeBase.generated.h"

class ASpaceWarCharacter;
class AMatchPlayerControllerBase;
class AGameStateMatchGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayerDead, AController*, InstegatedBy, AController*, LoserController, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMatchEnded, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerPostLogin, APlayerController*, PlayerConttroller);

UCLASS(Abstract)
class SPACEWAR_API AMatchGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AMatchGameModeBase();

	virtual void CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser);
	virtual void SpawnCharacter(AMatchPlayerControllerBase* Controller, ASpaceWarCharacter*& SpawnCharacter, const FVector& Location);
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual APawn* SpawnSpectator(AController* PossessController, const FVector& Location);

protected:

	virtual void LaunchGameTimer();
	virtual void MatchEnded(const FString& Reason);
	virtual void RespawnPlayer(AController* RespawnController, float const Time) {}
	virtual void RespawnPlayer(AController* RespawnController) {}
	virtual void TickTime(AGameStateMatchGame* MatchPlayerState);
	virtual void BeginPlay() override;
	
public:

	FPlayerDead OnPlayerDead;

	UPROPERTY()
	FMatchEnded OnMatchEnded;

	UPROPERTY()
	FPlayerPostLogin OnPlayerPostLogin;

protected:

	UPROPERTY(EditAnywhere)
	int32 PointForWin;

	FTimerHandle TimeMatchHandle;
};
