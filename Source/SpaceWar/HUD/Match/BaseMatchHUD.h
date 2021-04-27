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

public:

	ABaseMatchHUD();

protected:
	
	FMatchWidgetData* MatchWidgetData;
	EMatchData MatchType;

	UPROPERTY()
	UMatchWidgetDataAsset* AssetData;
};
