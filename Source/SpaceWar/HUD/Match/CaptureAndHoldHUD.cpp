// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureAndHoldHUD.h"

#include "Kismet/GameplayStatics.h"
#include "SpaceWaR/GameStates/Match/OnlinetMatchGameStateBase.h"
#include "SpaceWar/Interfaces/PreparationWidgetInterface.h"
#include "SpaceWar/PlayerControllers/Match/Last/CaptureHoldController.h"

ACaptureAndHoldHUD::ACaptureAndHoldHUD()
{
	MatchType = EMatchData::CaptureAndHold;
}

void ACaptureAndHoldHUD::BeginPlay()
{
	Super::BeginPlay();

	auto const GS = Cast<AOnlinetMatchGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GS->OnPreMatchEnd.AddDynamic(this, &ACaptureAndHoldHUD::PreMatchEnd);
	GS->OnMatchEnd.AddDynamic(this, &ACaptureAndHoldHUD::CreateMatchEndWidget);
	
	Cast<ACaptureHoldController>(GetOwningPlayerController())->OnPreparationSpawnPlayer.AddDynamic(this, &ACaptureAndHoldHUD::PreparationSpawnCharacter);
}

void ACaptureAndHoldHUD::PreMatchEnd(const FString& Reason, ETeam WinnerTeam)
{
	CreatePreMatchEnd(Reason, WinnerTeam);
}

void ACaptureAndHoldHUD::PreparationSpawnCharacter()
{
	if(!PreparationWidget)
	{
		RemoveSpectatorWidgets();
		CreatePreparationWidget();
		auto f = [&]() -> void
		{
			auto const PC = Cast<ACaptureHoldController>(GetOwningPlayerController());
			PC->SpawnPlayerByPoint(IPreparationWidgetInterface::Execute_FindSpawnPointNumber(PreparationWidget));
			RemovePreparationWidget();
		};
		FTimerDelegate TimerDel;
		FTimerHandle TimerHandle;
		TimerDel.BindLambda(f);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 5.f, false);
	}
}

void ACaptureAndHoldHUD::CreateSpectatorWidgets()
{
	Super::CreateSpectatorWidgets();
}
