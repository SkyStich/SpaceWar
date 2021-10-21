// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureFlagController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/GameModes/Match/LastGameMode/CaptureHoldGameMode.h"
#include "SpaceWar/PlayerStart/CaptureFlagPlayerStart.h"

void ACaptureFlagController::BeginPlay()
{
	Super::BeginPlay();

	auto const GS = Cast<ACaptureOfFlagGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if(GS)
	{
		GS->OnRoundEnded.AddDynamic(this, &ACaptureFlagController::RoundEnded);
		GS->OnRoundStarted.AddDynamic(this, &ACaptureFlagController::RoundStarted);
		
		if(GetLocalRole() == ROLE_Authority)
		{
			GS->OnRoundPreparation.AddDynamic(this, &ACaptureFlagController::PreparationRound);
		}
	}
	else
	{
		UE_LOG(LogPlayerController, Warning, TEXT("Game state field cast on ACaptureHoldGamestate -- ACaptureHoldController::BeginPlay()"), *GetName());
	}
}

void ACaptureFlagController::RoundEnded(const FString& Reason, ETeam WinnerTeam, EReasonForEndOfRound ReasonEndOfRound)
{
	if(GetCharacter())
	{
		GetCharacter()->GetCharacterMovement()->StopMovementImmediately();
		GetCharacter()->GetCharacterMovement()->DisableMovement();
		
		if(GetLocalRole() != ROLE_Authority)
			GetCharacter()->InputComponent->ClearActionBindings();
	}
}
	
void ACaptureFlagController::RoundStarted(ETeam SecurityTeam)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		/** Get All Player Start in current world */
		for(TActorIterator<ACaptureFlagPlayerStart> It(GetWorld(), ACaptureFlagPlayerStart::StaticClass()); It; ++It)
		{
			ACaptureFlagPlayerStart* Temp = *It;

			bool const bIsSecurity = IGetPlayerTeamInterface::Execute_FindPlayerTeam(PlayerState) == SecurityTeam;
			
			/** Find Free points */
			if(Temp->CheckOnFreePoints() && bIsSecurity == Temp->GetIsSecurityTeam())
			{
				SpawnPlayer(Temp->GetActorLocation());
				return;
			}
		}
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
		GM->SpawnSpectator(this, TempPawn->GetActorLocation(), GetControlRotation());
		TempPawn->Destroy();
	}
}


