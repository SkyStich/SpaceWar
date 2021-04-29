// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlinetMatchGameStateBase.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"

void AOnlinetMatchGameStateBase::NewPlayerLogin(APlayerController* PC)
{
	Super::NewPlayerLogin(PC);

	SetTeamForPlayer(PC);
}

void AOnlinetMatchGameStateBase::SetTeamForPlayer(APlayerController* PC)
{
	auto const OnlinePlayerState = Cast<AOnlinePlayerStateBase>(PC->PlayerState);
	if(!OnlinePlayerState) return;

	if((PlayerArray.Num() % 2) == 0)
	{
		OnlinePlayerState->SetTeam(ETeam::TeamB);
	}
	else
	{
		OnlinePlayerState->SetTeam(ETeam::TeamA);	
	}
}

void AOnlinetMatchGameStateBase::AutoBalanceTeam()
{
	
}

