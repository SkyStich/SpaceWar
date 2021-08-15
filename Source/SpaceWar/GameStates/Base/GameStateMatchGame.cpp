// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateMatchGame.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/GameModes/Match/Base/MatchGameModeBase.h"
#include "SpaceWar/Interfaces/GetPlayerLoginInterface.h"

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

void AGameStateMatchGame::MatchFinish_Implementation(const FString& Reason, ETeam WinnerTeam)
{
	OnMatchEnd.Broadcast(Reason, WinnerTeam);
}

void AGameStateMatchGame::PreMatchFinish_Implementation(const FString& Reason, ETeam WinnerTeam)
{
	OnPreMatchEnd.Broadcast(Reason, WinnerTeam);
}

void AGameStateMatchGame::PlayerDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	const FString InstigatorName = IGetPlayerLoginInterface::Execute_PlayerLoginFromController(InstigatorController);
	const FString LoserName = IGetPlayerLoginInterface::Execute_PlayerLoginFromController(LoserController);
	NetMulticast_OnPlayerDead(InstigatorName, LoserName, DamageCauser);
}

void AGameStateMatchGame::NetMulticast_OnPlayerDead_Implementation(const FString& InstigatorName, const FString& LoserName, AActor* DamageCauser)
{
	if(GetLocalRole() != ROLE_Authority)
		OnPlayerDead.Broadcast(InstigatorName, LoserName, DamageCauser);
}
