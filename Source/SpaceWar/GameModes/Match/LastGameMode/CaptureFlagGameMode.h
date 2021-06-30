// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/GameModes/Match/OnlineMatchGameModeBase.h"
#include "SpaceWar/GameStates/Match/CaptureOfFlagGameState.h"
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
	virtual void UpdateTeamPoints(const int32 Value, ETeam Team, EReasonForEndOfRound ReasonEndOfRound);

protected:

	virtual void CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser) override;
	virtual void BeginPlay() override;
	virtual void RespawnPlayer(AController* RespawnController) override;
	virtual void TickTime(AOnlinetMatchGameStateBase* MatchPlayerState) override;

	UFUNCTION()
    virtual void RoundStarted();

	UFUNCTION()
    virtual void RoundEnded(const FString& Reason, ETeam WinnerTeam, EReasonForEndOfRound ReasonEndOfRound);

	UFUNCTION()
    virtual void PreparationRound();	
};
