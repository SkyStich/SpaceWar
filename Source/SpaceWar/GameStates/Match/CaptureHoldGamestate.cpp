// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureHoldGamestate.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "SpaceWar/GameModes/Match/LastGameMode/CaptureHoldGameMode.h"

ACaptureHoldGamestate::ACaptureHoldGamestate()
{
	
}

void ACaptureHoldGamestate::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		Cast<ACaptureHoldGameMode>(AuthorityGameMode)->OnPreMatchEnded.AddDynamic(this, &ACaptureHoldGamestate::PreparationEndMatch);
	}
}


void ACaptureHoldGamestate::MatchFinish_Implementation(const FString& Reason, ETeam WinnerTeam)
{
	Super::MatchFinish_Implementation(Reason, WinnerTeam);
}

void ACaptureHoldGamestate::IncrementTime()
{
	Super::IncrementTime();
}

void ACaptureHoldGamestate::PreparationEndMatch_Implementation(const FString& Reason, ETeam WinnerTeam)
{
	OnPreMatchEnd.Broadcast(Reason, WinnerTeam);
}


