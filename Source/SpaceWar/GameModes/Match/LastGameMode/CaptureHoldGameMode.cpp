// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureHoldGameMode.h"
#include "SpaceWar/PlayerControllers/Match/Last/CaptureHoldController.h"
#include "SpaceWar/GameStates/Match/CaptureHoldGamestate.h"

ACaptureHoldGameMode::ACaptureHoldGameMode()
{
	PointForWin = 1000;
}

void ACaptureHoldGameMode::BeginPlay()
{
	Super::BeginPlay();

	LaunchGameTimer();
}

void ACaptureHoldGameMode::CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	Super::CharDead(InstigatorController, LoserController, DamageCauser);

	RespawnPlayer(LoserController, 5.f);
}

void ACaptureHoldGameMode::RespawnPlayer(AController* LoserController, float const Time)
{
	auto const LosController = Cast<ACaptureHoldController>(LoserController);
	if(LosController)
	{
		LosController->LaunchRespawnTimer(Time);
	}
}

void ACaptureHoldGameMode::MatchEnded(const FString& Reason, ETeam WinnerTeam)
{
	Super::MatchEnded(Reason, WinnerTeam);

	for(auto& ByArray : GameState->PlayerArray)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(ByArray->GetOwner());
	}
}

void ACaptureHoldGameMode::TickTime(AOnlinetMatchGameStateBase* MatchGameState)
{
	Super::TickTime(MatchGameState);
	if(MatchGameState->GetCurrentMatchTime() <= 0)
	{
		ETeam const Winner = MatchGameState->GetTeamPointsA()  > MatchGameState->GetTeamPointsB() ? ETeam::TeamA : MatchGameState->GetTeamPointsA() < MatchGameState->GetTeamPointsB() ? ETeam::TeamB : ETeam::NoneTeam;
		FString const TeamReason = Winner == ETeam::NoneTeam ? "No winners" : "";
		MatchEnded("Time leave" + TeamReason, Winner);
		GetWorld()->GetTimerManager().ClearTimer(TimeMatchHandle);
	}
}