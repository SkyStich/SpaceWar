// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpaceWar/DataAssets/MatchWidgetDataAsset.h"
#include "BaseMatchHUD.generated.h"

UCLASS(BlueprintType)
class SPACEWAR_API ABaseMatchHUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void NewOwningPlayerPawn(APawn* NewPawn);

	virtual void CreateCharacterWidgets();
	virtual void RemoveCharacterWidgets();
	virtual void CreateSpectatorWidgets();
	virtual void RemoveSpectatorWidgets();
	
	virtual void CreateTabMenu();

public:

	ABaseMatchHUD();
	
	UFUNCTION(BlueprintPure)
	UUserWidget* GetMainWidget() const { return MainHudWidget; }
	 
	void ShowTabMenu();
	void HiddenTabMenu();

protected:
	
	FMatchWidgetData* MatchWidgetData;
	EMatchData MatchType;

	UPROPERTY()
	UMatchWidgetDataAsset* AssetData;

	UPROPERTY()
	UUserWidget* MainHudWidget;

	UPROPERTY()
	UUserWidget* TabMenuWidget;
};
