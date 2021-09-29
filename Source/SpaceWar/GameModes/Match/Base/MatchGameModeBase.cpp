// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameModeBase.h"
#include "SpaceWar/Singleton/BaseSingleton.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/BPFLibrary/ServerManipulationLibrary.h"
#include "SpaceWar/GameStates/Base/GameStateMatchGame.h"
#include "SpaceWar/Spectator/Base/BaseMatchSpectator.h"

AMatchGameModeBase::AMatchGameModeBase()
{
	PointForWin = 1000;

	DataBaseComponent = CreateDefaultSubobject<UGameServerDataBaseComponent>(TEXT("DataBaseComponent"));
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
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Param.Owner = PossessController;
	Param.Instigator = PossessController->GetPawn();
	auto Spectator = GetWorld()->SpawnActor<ABaseMatchSpectator>(ABaseMatchSpectator::StaticClass(), Location, FRotator::ZeroRotator, Param);
	if(!Spectator) return;
		
	Spectator->SetOwner(Spectator);
	Spectator->SetInstigator(Spectator);
	PossessController->Possess(Spectator);
}

void AMatchGameModeBase::AsyncSpawnSpectatorComplete(FSoftObjectPath Reference, FTransform SpawnTransform, AController* Controller)
{
	UClass* ActorClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), Controller, *Reference.ToString()));
	if(!ActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor class is NULL -- AMatchGameModeBase::OnAsyncSpawnSpectatorComplete"), *Reference.ToString());
	}
	else
	{
		ABaseMatchSpectator* Spectator = nullptr;
		
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
	if(Controller->GetPlayerClass().IsNull() || !Controller)
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

void AMatchGameModeBase::GameFinish(FString Reason, ETeam WinnerTeam)
{
	OnMatchEnded.Broadcast(Reason, WinnerTeam);
}

void AMatchGameModeBase::MatchEnded(const FString& Reason, ETeam WinnerTeam)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(GameState);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	OnPreMatchEnded.Broadcast(Reason, WinnerTeam);
	
	DataBaseComponent->RemoveServerFromDataBase();
	
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &AMatchGameModeBase::GameFinish, Reason, WinnerTeam);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 10.f, false);
}

void AMatchGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	OnPlayerPostLogin.Broadcast(NewPlayer);
}

void AMatchGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if(GameState->PlayerArray.Num() <= 0)
	{
		DataBaseComponent->ShutDownServer();
	}
}
