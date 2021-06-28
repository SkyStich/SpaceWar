// Fill out your copyright notice in the Description page of Project Settings.


#include "HoldOfFlagHUD.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceWar/GameStates/Match/CaptureOfFlagGameState.h"

void AHoldOfFlagHUD::BeginPlay()
{
	Super::BeginPlay();

	PreparationRound();

	auto const GameState = Cast<ACaptureOfFlagGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if(GameState)
	{
		GameState->OnRoundPreparation.AddDynamic(this, &AHoldOfFlagHUD::PreparationRound);
		GameState->OnRoundStarted.AddDynamic(this, &AHoldOfFlagHUD::StartRound);
		GameState->OnRoundEnded.AddDynamic(this, &AHoldOfFlagHUD::StopRound);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to ACaptureOfFlagGameState is null -- AHoldOfFlagHUD::BeginPlay "), *GetFullName());
	}
}

void AHoldOfFlagHUD::PreparationRound()
{
	CreatePreparationWidget();
	RemoveSpectatorWidgets();
	RemoveCharacterWidgets();
}

void AHoldOfFlagHUD::StartRound()
{
	RemovePreparationWidget();
}

void AHoldOfFlagHUD::StopRound()
{

}