// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMatchHUD.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

ABaseMatchHUD::ABaseMatchHUD()
{
	MatchType = EMatchData::EveryManForHimSelfGame;
	ConstructorHelpers::FObjectFinder<UMatchWidgetDataAsset>WidgetData(TEXT("/Game/ThirdPersonCPP/DataAssets/WidgetDataAsset"));
	if(WidgetData.Succeeded())
	{
		AssetData = WidgetData.Object;
	}
}

void ABaseMatchHUD::BeginPlay()
{
	Super::BeginPlay();

	MatchWidgetData = AssetData->GetWidgetData(MatchType);
	
	CreateTabMenu();
	GetOwningPlayerController()->GetOnNewPawnNotifier().AddUObject(this, &ABaseMatchHUD::NewOwningPlayerPawn);
}

void ABaseMatchHUD::NewOwningPlayerPawn(APawn* NewPawn)
{
	if(GetOwningPlayerController()->GetCharacter())
	{
		//Character
		RemoveSpectatorWidgets();
		CreateCharacterWidgets();
	}
	else
	{
		//Spectator
		RemoveCharacterWidgets();
		CreateSpectatorWidgets();
	}
}

void ABaseMatchHUD::CreateTabMenu()
{
	TabMenuWidget = AssetData->SyncCreateWidget(GetWorld(), MatchWidgetData->StatisticMatchGrid, GetOwningPlayerController());
	TabMenuWidget->AddToViewport();
	TabMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ABaseMatchHUD::CreateCharacterWidgets()
{
	MainHudWidget = AssetData->SyncCreateWidget(GetWorld(), MatchWidgetData->HUD, GetOwningPlayerController());
	MainHudWidget->AddToViewport();
}

void ABaseMatchHUD::RemoveCharacterWidgets()
{
	if(MainHudWidget)
	{
		MainHudWidget->RemoveFromParent();
		MainHudWidget = nullptr;
	}
}

void ABaseMatchHUD::CreateSpectatorWidgets()
{
}

void ABaseMatchHUD::RemoveSpectatorWidgets()
{
	if(MainHudWidget)
	{
		MainHudWidget->RemoveFromParent();
		MainHudWidget = nullptr;
	}
}

void ABaseMatchHUD::ShowTabMenu()
{
	TabMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

void ABaseMatchHUD::HiddenTabMenu()
{
	TabMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
}



