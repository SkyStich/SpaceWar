// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMatchHUD.h"
#include "SpaceWar/GameStates/Match/CaptureOfFlagGameState.h"
#include "HoldOfFlagHUD.generated.h"

UCLASS()
class SPACEWAR_API AHoldOfFlagHUD : public ABaseMatchHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void NewOwningPlayerPawn(APawn* NewPawn) override;

	UFUNCTION()
	void PreparationRound();

	UFUNCTION()
	void StartRound(ETeam SecurityTeam);

	UFUNCTION()
	void StopRound(const FString& Reason, ETeam WinnerTeam, EReasonForEndOfRound ReasonEndOfRound);
};
