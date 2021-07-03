// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureAndHoldHUD.h"


#include "SpaceWar/Interfaces/PreparationWidgetInterface.h"
#include "SpaceWar/PlayerControllers/Match/Last/CaptureHoldController.h"

ACaptureAndHoldHUD::ACaptureAndHoldHUD()
{
	MatchType = EMatchData::CaptureAndHold;
}

void ACaptureAndHoldHUD::BeginPlay()
{
	Super::BeginPlay();

	auto const PS = Cast<ACaptureHoldController>(GetOwningPlayerController());
	if(PS)
	{
		PS->OnPreparationSpawnPlayer.AddDynamic(this, &ACaptureAndHoldHUD::PreparationSpawnCharacter);
	}
}

void ACaptureAndHoldHUD::PreparationSpawnCharacter()
{
	if(!PreparationWidget)
	{
		RemoveSpectatorWidgets();
		CreatePreparationWidget();
		auto f = [&]()
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
