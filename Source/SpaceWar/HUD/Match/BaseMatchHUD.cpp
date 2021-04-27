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
	auto const Widget = AssetData->SyncCreateWidget(GetWorld(), MatchWidgetData->HUD, GetOwningPlayerController());
	if(Widget)
	{
		Widget->AddToViewport();
	}
}
