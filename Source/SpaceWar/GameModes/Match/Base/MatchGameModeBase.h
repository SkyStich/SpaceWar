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

UCLASS()
class SPACEWAR_API AMatchGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	void TickTime(AGameStateMatchGame* MatchPlayerState);

public:

	virtual void CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser);
	virtual void SpawnCharacter(AMatchPlayerControllerBase* Controller, ASpaceWarCharacter*& SpawnCharacter);
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:

	virtual APawn* SpawnSpectator(AController* PossessController, const FVector& Location);
	virtual void LaunchGameTimer();
	void MatchEnded(const FString& Reason);

	virtual void BeginPlay() override;
public:

	FPlayerDead OnPlayerDead;

	UPROPERTY()
	FMatchEnded OnMatchEnded;

	UPROPERTY()
	FPlayerPostLogin OnPlayerPostLogin;

private:

	FTimerHandle TimeMatchHandle;
};
