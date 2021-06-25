// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameModeBase.h"
#include "SpaceWar/Singleton/BaseSingleton.h"
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
}

void AMatchGameModeBase::CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	SpawnSpectator(LoserController, LoserController->GetPawn()->GetActorLocation());
	
	OnPlayerDead.Broadcast(InstigatorController, LoserController, DamageCauser);
}

void AMatchGameModeBase::SpawnSpectator(AController* PossessController, const FVector& Location)
{
	TAssetSubclassOf<ABaseMatchSpectator> const Ptr = ABaseMatchSpectator::StaticClass();
	FTransform const Transform(FRotator::ZeroRotator, Location, FVector(1.f));
	FStreamableManager& StreamableManager = UBaseSingleton::Get().AssetLoader;
	FSoftObjectPath const Ref = Ptr.ToSoftObjectPath();
	StreamableManager.RequestAsyncLoad(Ref, FStreamableDelegate::CreateUObject(this, &AMatchGameModeBase::AsyncSpawnSpectatorComplete, Ref, Transform, PossessController));
}

void AMatchGameModeBase::AsyncSpawnSpectatorComplete(FSoftObjectPath Reference, FTransform SpawnTransform, AController* Controller)
{
	ABaseMatchSpectator* Spectator = nullptr;
	UClass* ActorClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), Controller, *Reference.ToString()));
	if(!ActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor class is NULL -- AMatchGameModeBase::OnAsyncSpawnSpectatorComplete"), *Reference.ToString());
	}
	else
	{
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Param.Owner = Controller;
		Param.Instigator = Controller->GetPawn();
		Spectator = GetWorld()->SpawnActor<ABaseMatchSpectator>(ActorClass, SpawnTransform, Param);
		if(!Spectator) return;
		
		Spectator->SetOwner(Spectator);
		Spectator->SetInstigator(Spectator);
		Controller->Possess(Spectator);
	}
}

void AMatchGameModeBase::SpawnCharacter(AMatchPlayerControllerBase* Controller, const FVector& Location)
{
	if(Controller->GetPlayerClass() == nullptr)
	{
		FString const InstigatorName = *GetWorld()->GetFullName();
		UE_LOG(LogGameMode, Warning, TEXT("Player class is null -- AMatchPlayerControllerBase::PlayerClass"), *InstigatorName);
		return;
	}
	FTransform const Transform(FRotator::ZeroRotator, Location, FVector(1.f));
	FStreamableManager& StreamableManager = UBaseSingleton::Get().AssetLoader;
	FSoftObjectPath const Ref = Controller->GetPlayerClass().ToSoftObjectPath();
	StreamableManager.RequestAsyncLoad(Ref, FStreamableDelegate::CreateUObject(this, &AMatchGameModeBase::AsyncSpawnPlayerCharacterComplete, Ref, Transform, Controller));
}

void AMatchGameModeBase::AsyncSpawnPlayerCharacterComplete(FSoftObjectPath Reference, FTransform SpawnTransform, AMatchPlayerControllerBase* Controller)
{
	UClass* ActorClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), Controller, *Reference.ToString()));
	if(!ActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor class is NULL -- AMatchGameModeBase::AsyncSpawnPlayerCharacterComplete"), *Reference.ToString());
		return;
	}

	ASpaceWarCharacter* SpawnCharacter = nullptr;
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnCharacter = GetWorld()->SpawnActor<ASpaceWarCharacter>(ActorClass, SpawnTransform, Params);

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