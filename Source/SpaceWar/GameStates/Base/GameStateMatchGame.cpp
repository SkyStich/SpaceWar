// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateMatchGame.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/GameModes/Match/Base/MatchGameModeBase.h"

AGameStateMatchGame::AGameStateMatchGame()
{
	MaxMatchTime = 60;
	CurrentMatchTime = MaxMatchTime;
}

void AGameStateMatchGame::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		auto const GameMode = Cast<AMatchGameModeBase>(AuthorityGameMode);
		GameMode->OnMatchEnded.AddDynamic(this, &AGameStateMatchGame::MatchFinish);
		GameMode->OnPlayerPostLogin.AddDynamic(this, &AGameStateMatchGame::NewPlayerLogin);
		GameMode->OnPlayerDead.AddDynamic(this, &AGameStateMatchGame::PlayerDead);
	}
}

void AGameStateMatchGame::NewPlayerLogin(APlayerController* PC)
{
	
}

void AGameStateMatchGame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameStateMatchGame, CurrentMatchTime);
}

void AGameStateMatchGame::IncrementTime()
{
	CurrentMatchTime--;
}

void AGameStateMatchGame::MatchFinish_Implementation(const FString& Reason)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, Reason);
}
