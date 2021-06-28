// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureFlagGameMode.h"
#include "SpaceWar/GameStates/Match/CaptureOfFlagGameState.h"

ACaptureFlagGameMode::ACaptureFlagGameMode()
{
	PointForWin = 5;
}

void ACaptureFlagGameMode::CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	Super::CharDead(InstigatorController, LoserController, DamageCauser);
}

void ACaptureFlagGameMode::UpdateTeamPoints(const int32 Value, ETeam Team)
{
	Super::UpdateTeamPoints(Value, Team);
}

void ACaptureFlagGameMode::RoundStarted()
{
	LaunchGameTimer();
}

void ACaptureFlagGameMode::RoundEnded()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeMatchHandle);
}

void ACaptureFlagGameMode::PreparationRound()
{
	
}

void ACaptureFlagGameMode::BeginPlay()
{
	Super::BeginPlay();

	auto const GS = Cast<ACaptureOfFlagGameState>(GameState);
	GS->OnRoundEnded.AddDynamic(this, &ACaptureFlagGameMode::RoundEnded);
	GS->OnRoundStarted.AddDynamic(this, &ACaptureFlagGameMode::RoundStarted);
	GS->OnRoundPreparation.AddDynamic(this, &ACaptureFlagGameMode::PreparationRound);
}

void ACaptureFlagGameMode::RespawnPlayer(AController* RespawnController)
{
	
}

void ACaptureFlagGameMode::TickTime(AGameStateMatchGame* MatchPlayerState)
{
	Super::TickTime(MatchPlayerState);

	if(MatchPlayerState->GetCurrentMatchTime() <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeMatchHandle);
	}
}
