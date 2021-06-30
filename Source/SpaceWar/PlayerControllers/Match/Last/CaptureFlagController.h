// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "SpaceWar/GameStates/Match/CaptureOfFlagGameState.h"
#include "CaptureFlagController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ACaptureFlagController : public AMatchPlayerControllerBase
{
	GENERATED_BODY()
	
protected:
    
	virtual void BeginPlay() override;
	virtual bool SpawnPlayer(const FVector& Location) override;
    
	UFUNCTION()
    virtual void RoundEnded(const FString& Reason, ETeam WinnerTeam, EReasonForEndOfRound ReasonEndOfRound);
    
	UFUNCTION()
    virtual void RoundStarted();

	UFUNCTION()
	virtual void PreparationRound();
};
