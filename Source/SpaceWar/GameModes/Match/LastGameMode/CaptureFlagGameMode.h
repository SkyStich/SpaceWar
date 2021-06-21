// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/GameModes/Match/OnlineMatchGameModeBase.h"
#include "CaptureFlagGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ACaptureFlagGameMode : public AOnlineMatchGameModeBase
{
	GENERATED_BODY()
public:
	
	ACaptureFlagGameMode();
	virtual void UpdateTeamPoints(const int32 Value, ETeam Team) override;

protected:

	virtual void CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser) override;
	virtual void BeginPlay() override;
	virtual void RespawnPlayer(AController* RespawnController) override;
	virtual void TickTime(AGameStateMatchGame* MatchPlayerState) override;

	UFUNCTION()
    virtual void RoundStarted();

	UFUNCTION()
    virtual void RoundEnded();

	UFUNCTION()
    virtual void PreparationRound();	
};
