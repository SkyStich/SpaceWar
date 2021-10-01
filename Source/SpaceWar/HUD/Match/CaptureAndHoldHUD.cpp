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
	
	ShowPreparationWidget();

	auto const GS = Cast<AOnlinetMatchGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GS->OnPreMatchEnd.AddDynamic(this, &ACaptureAndHoldHUD::PreMatchEnd);
	GS->OnMatchEnd.AddDynamic(this, &ACaptureAndHoldHUD::CreateMatchEndWidget);
	
	Cast<ACaptureHoldController>(GetOwningPlayerController())->OnPreparationSpawnPlayer.AddDynamic(this, &ACaptureAndHoldHUD::PreparationSpawnCharacter);
}

void ACaptureAndHoldHUD::NewOwningPlayerPawn(APawn* Pawn)
{
	Super::NewOwningPlayerPawn(Pawn);

	if(GetOwningPlayerController()->GetCharacter())
	{
		RemovePreparationWidget();
	}
	else
	{
		CreatePreparationWidget();
		HiddenPreparationWidget();
	}
}

void ACaptureAndHoldHUD::PreMatchEnd(const FString& Reason, ETeam WinnerTeam)
{
	CreatePreMatchEnd(Reason, WinnerTeam);
	RemoveCharacterWidgets();
	RemoveSpectatorWidgets();
}

void ACaptureAndHoldHUD::PreparationSpawnCharacter()
{
	if(!PreparationWidget->IsVisible())
	{
		RemoveSpectatorWidgets();
		ShowPreparationWidget();
	}
}

void ACaptureAndHoldHUD::CreateSpectatorWidgets()
{
	Super::CreateSpectatorWidgets();
}

void ACaptureAndHoldHUD::ShowPreparationWidget()
{
	if(PreparationWidget)
	{
		PreparationWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACaptureAndHoldHUD::HiddenPreparationWidget()
{
	if(PreparationWidget)
	{
		PreparationWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ACaptureAndHoldHUD::CreatePreparationWidget()
{
	if(!PreparationWidget)
	{
		PreparationWidget = AssetData->SyncCreateWidget<UUserWidget>(GetWorld(), MatchWidgetData->PreparationMatch, GetOwningPlayerController());
		PreparationWidget->SetVisibility(ESlateVisibility::Collapsed);
		PreparationWidget->AddToViewport();
	}
}
