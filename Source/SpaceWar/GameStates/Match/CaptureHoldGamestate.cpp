// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureHoldGamestate.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

ACaptureHoldGamestate::ACaptureHoldGamestate()
{
}

void ACaptureHoldGamestate::MatchFinish_Implementation(const FString& Reason, ETeam WinnerTeam)
{
	Super::MatchFinish_Implementation(Reason, WinnerTeam);
}
