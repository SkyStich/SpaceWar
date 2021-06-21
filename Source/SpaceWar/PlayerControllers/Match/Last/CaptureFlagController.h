// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
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
    virtual void RoundEnded();
    
	UFUNCTION()
    virtual void RoundStarted();

	UFUNCTION()
	virtual void PreparationRound();
};
