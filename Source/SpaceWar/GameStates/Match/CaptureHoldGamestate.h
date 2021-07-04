// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/GameStates/Match/OnlinetMatchGameStateBase.h"
#include "CaptureHoldGamestate.generated.h"



UCLASS()
class SPACEWAR_API ACaptureHoldGamestate : public AOnlinetMatchGameStateBase
{
	GENERATED_BODY()

public:

	ACaptureHoldGamestate();

	virtual void IncrementTime() override;

protected:

	UFUNCTION(NetMulticast, Reliable)
	void PreparationEndMatch(const FString& Reason, ETeam WinnerTeam);
	
	virtual void MatchFinish_Implementation(const FString& Reason, ETeam WinnerTeam) override;
	virtual void BeginPlay() override;
};
