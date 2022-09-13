// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameModeBase.h"
#include "SpaceWar/Singleton/BaseSingleton.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "SpaceWar/Components/GameMode/GameDataBaseComponent.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/GameStates/Base/GameStateMatchGame.h"
#include "SpaceWar/Spectator/Base/BaseMatchSpectator.h"

AMatchGameModeBase::AMatchGameModeBase()
{
	PointForWin = 1000;

	GameDataBaseComponent = CreateDefaultSubobject<UGameDataBaseComponent>(TEXT("GameDataBaseComponent"));
}

void AMatchGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GameDataBaseComponent->OnForcedServerShutdown.AddDynamic(this, &AMatchGameModeBase::OnForcedServerShutdownEvent);
}

void AMatchGameModeBase::OnForcedServerShutdownEvent()
{
	OnPreServerShutdown.Broadcast();
}

void AMatchGameModeBase::CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	SpawnSpectator(LoserController, InstigatorController->GetPawn()->GetActorLocation(), LoserController->GetControlRotation());
	
	OnPlayerDead.Broadcast(InstigatorController, LoserController, DamageCauser);
}

void AMatchGameModeBase::SpawnSpectator(AController* PossessController, const FVector& Location, const FRotator& Rotation)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Param.Owner = PossessController;
	Param.Instigator = PossessController->GetPawn();
	auto Spectator = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Location + FVector(0.f, 0.f, 0.f), Rotation, Param);
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
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Param.Owner = Controller;
		Param.Instigator = Controller->GetPawn();
		ABaseMatchSpectator* NewSpectator = GetWorld()->SpawnActor<ABaseMatchSpectator>(ActorClass, SpawnTransform, Param);
		if(!NewSpectator) return;
		
		NewSpectator->SetOwner(NewSpectator);
		NewSpectator->SetInstigator(NewSpectator);
		Controller->Possess(NewSpectator);
	}
}

void AMatchGameModeBase::SpawnCharacter(AMatchPlayerControllerBase* Controller, const FVector& Location)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto SpawnCharacter = GetWorld()->SpawnActor<ASpaceWarCharacter>(Controller->GetPlayerClass(), Location, FRotator::ZeroRotator, Params);

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

#if UE_SERVER
	GameDataBaseComponent->RemoveServerFromDataBase();
#endif
	
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

void AMatchGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AMatchGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]() -> void
	{
		if(GameState->PlayerArray.Num() <= 0)
		{
			GameDataBaseComponent->ShutDownServer("All player disconnect from server");
		}
	});
	FTimerHandle ShutDownTimer;
	GetWorld()->GetTimerManager().SetTimer(ShutDownTimer, TimerDel, 5.f, false);

}