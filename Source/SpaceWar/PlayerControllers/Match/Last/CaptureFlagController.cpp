// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureFlagController.h"
#include "SpaceWar/GameStates/Match/CaptureOfFlagGameState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/GameModes/Match/LastGameMode/CaptureHoldGameMode.h"

void ACaptureFlagController::BeginPlay()
{
	Super::BeginPlay();

	auto const GS = Cast<ACaptureOfFlagGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if(GS)
	{
		GS->OnRoundEnded.AddDynamic(this, &ACaptureFlagController::RoundEnded);
		if(GetLocalRole() == ROLE_Authority)
		{
			GS->OnRoundStarted.AddDynamic(this, &ACaptureFlagController::RoundStarted);
			GS->OnRoundPreparation.AddDynamic(this, &ACaptureFlagController::PreparationRound);
		}
	}
	else
	{
		UE_LOG(LogPlayerController, Warning, TEXT("Game state field cast on ACaptureHoldGamestate -- ACaptureHoldController::BeginPlay()"), *GetName());
	}
}

void ACaptureFlagController::RoundEnded()
{
	if(GetCharacter())
	{
		GetCharacter()->GetCharacterMovement()->StopMovementImmediately();
		GetCharacter()->GetCharacterMovement()->DisableMovement();
		
		if(GetLocalRole() != ROLE_Authority)
			GetCharacter()->InputComponent->ClearActionBindings();
	}
}

void ACaptureFlagController::RoundStarted()
{
	/** Test */
	if(GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> Test;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Test);
		SpawnPlayer(Test[0]->GetActorLocation());
	}
}

bool ACaptureFlagController::SpawnPlayer(const FVector& Location)
{
	return Super::SpawnPlayer(Location);
}

void ACaptureFlagController::PreparationRound()
{
	if(GetCharacter())
	{
		/** if controller controlled PlayerCharacter, Spawn spectator and destroy player character */
		auto const GM = Cast<AMatchGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		auto const TempPawn = GetCharacter();
		GM->SpawnSpectator(this, GetCharacter()->GetActorLocation());
		TempPawn->Destroy();
	}
}


