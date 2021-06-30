// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureFlagGameMode.h"

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

void ACaptureFlagGameMode::UpdateTeamPoints(const int32 Value, ETeam Team,EReasonForEndOfRound ReasonEndOfRound)
{
	if(GetGameState<ACaptureOfFlagGameState>()->UpdateTeamPoints(Team, Value, ReasonEndOfRound) >= PointForWin)
	{
		FString const ReasonMessage = FString::Printf(TEXT("Max amount points. %d - winner"), Team); 
		MatchEnded(ReasonMessage, Team);
	}
}

void ACaptureFlagGameMode::RoundStarted()
{
	LaunchGameTimer();
}

void ACaptureFlagGameMode::RoundEnded(const FString& Reason, ETeam WinnerTeam, EReasonForEndOfRound ReasonEndOfRound)
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

void ACaptureFlagGameMode::TickTime(AOnlinetMatchGameStateBase* MatchPlayerState)
{
	Super::TickTime(MatchPlayerState);

	if(MatchPlayerState->GetCurrentMatchTime() <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimeMatchHandle);
	}
}
