// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MatchWidgetDataAsset.generated.h"

UENUM(BlueprintType)
enum class EMatchData : uint8
{
	EveryManForHimSelfGame,
	CommandGame
};

USTRUCT(BlueprintType)
struct FMatchWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> HUD;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> StatisticMatchGrid;
};

UCLASS(Blueprintable)
class SPACEWAR_API UMatchWidgetDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	FMatchWidgetData* GetWidgetData(EMatchData MatchType) { return WidgetData.Find(MatchType); }
	UUserWidget* SyncCreateWidget(UObject* WorldContext, TSoftClassPtr<UUserWidget> WidgetClass, APlayerController* OwningPlayer);

private:

	UPROPERTY(EditAnywhere)
	TMap<EMatchData, FMatchWidgetData> WidgetData;
};
