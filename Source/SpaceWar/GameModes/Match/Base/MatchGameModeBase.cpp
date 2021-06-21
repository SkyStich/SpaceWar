// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/GameStates/Base/GameStateMatchGame.h"
#include "SpaceWar/Spectator/Base/BaseMatchSpectator.h"

AMatchGameModeBase::AMatchGameModeBase()
{
	PointForWin = 1000;
}

void AMatchGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	LaunchGameTimer();
}

void AMatchGameModeBase::CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	SpawnSpectator(LoserController, LoserController->GetPawn()->GetActorLocation());
	
	OnPlayerDead.Broadcast(InstigatorController, LoserController, DamageCauser);
}

APawn* AMatchGameModeBase::SpawnSpectator(AController* PossessController, const FVector& Location)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto const Spectator = GetWorld()->SpawnActor<ABaseMatchSpectator>(ABaseMatchSpectator::StaticClass(), Location, FRotator::ZeroRotator, Params);

	if(Spectator)
	{
		Spectator->SetOwner(Spectator);
		Spectator->SetInstigator(Spectator);
		PossessController->Possess(Spectator);
		return Spectator;
	}

	UE_LOG(LogGameMode, Error, TEXT("Spectator not spawn --SpawnSpectator : %d"), *GetFullName());
	return nullptr;
}

void AMatchGameModeBase::SpawnCharacter(AMatchPlayerControllerBase* Controller, ASpaceWarCharacter*& SpawnCharacter, const FVector& Location)
{
	FVector const Loc = Location;
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnCharacter = GetWorld()->SpawnActor<ASpaceWarCharacter>(Controller->GetPlayerClass(), Loc, FRotator::ZeroRotator, Params);

	if(SpawnCharacter)
	{
		SpawnCharacter->SetOwner(SpawnCharacter);
		SpawnCharacter->SetInstigator(SpawnCharacter);
		auto const TempPawn = Controller->GetPawn();
		Controller->Possess(SpawnCharacter);
		TempPawn->Destroy();
	}
}

void AMatchGameModeBase::LaunchGameTimer()
{
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &AMatchGameModeBase::TickTime, GetGameState<AGameStateMatchGame>());
	GetWorld()->GetTimerManager().SetTimer(TimeMatchHandle, TimerDel, 1.f, true);
}

void AMatchGameModeBase::TickTime(AGameStateMatchGame* MatchGameState)
{
	MatchGameState->IncrementTime();
}

void AMatchGameModeBase::MatchEnded(const FString& Reason)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(GameState);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	OnMatchEnded.Broadcast(Reason);
}

void AMatchGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	OnPlayerPostLogin.Broadcast(NewPlayer);
}

