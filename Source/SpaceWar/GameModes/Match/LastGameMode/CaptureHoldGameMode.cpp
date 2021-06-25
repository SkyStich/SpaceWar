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

void ACaptureHoldGameMode::MatchEnded(const FString& Reason)
{
	Super::MatchEnded(Reason);

	for(auto& ByArray : GameState->PlayerArray)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(ByArray->GetOwner());
	}
}

void ACaptureHoldGameMode::TickTime(AGameStateMatchGame* MatchGameState)
{
	Super::TickTime(MatchGameState);
	if(MatchGameState->GetCurrentMatchTime() <= 0)
	{
		MatchEnded("Time leave");
		GetWorld()->GetTimerManager().ClearTimer(TimeMatchHandle);
	}
}
