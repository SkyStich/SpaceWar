// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Blueprint/UserWidget.h"
#include "SpaceWar/UserWidget/Match/EndGameWidgetBase.h"
#include "SpaceWar/Singleton/BaseSingleton.h"
#include "MatchWidgetDataAsset.generated.h"

UENUM(BlueprintType)
enum class EMatchData : uint8
{
	Unknown,
	CaptureOfFlag,
	CaptureAndHold
};

USTRUCT(BlueprintType)
struct FMatchWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> HUD;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> PreparationMatch;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> StatisticMatchGrid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UEndGameWidgetBase> EndMatch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UEndGameWidgetBase> PreEndMatch;
};

UCLASS(Blueprintable)
class SPACEWAR_API UMatchWidgetDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	FMatchWidgetData* GetWidgetData(EMatchData MatchType) { return WidgetData.Find(MatchType); }
	
	template<class T>
	static T* SyncCreateWidget(UObject* WorldContext, TSoftClassPtr<UUserWidget> WidgetClass, APlayerController* OwningPlayer)
	{
		if(WidgetClass.IsNull())
		{
			FString const InstigatorName = WorldContext != nullptr ? WorldContext->GetFullName() : "Unknown";
			UE_LOG(LogAssetData, Error, TEXT("UUserWidget::SyncCreateWidget -- Asset ptr is null %d"), *InstigatorName);
			return nullptr;
		}

		FStreamableManager& StreamableManager = UBaseSingleton::Get().AssetLoader;
		FSoftObjectPath const Ref = WidgetClass.ToSoftObjectPath();
		StreamableManager.LoadSynchronous(WidgetClass);
	
		UClass* WidgetType = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *Ref.ToString()));
	
		if(WidgetType == nullptr)
		{
			return nullptr;
		}

		T* Widget = nullptr;
		if(OwningPlayer == nullptr)
		{
			UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
			Widget = CreateWidget<T>(World, WidgetType);
		}
		else
		{
			Widget = CreateWidget<T>(OwningPlayer, WidgetType);
		}
	
		if(Widget)
		{
			Widget->SetFlags(EObjectFlags::RF_StrongRefOnFrame);
		}
	
		return Widget;
	}
	
private:

	UPROPERTY(EditAnywhere)
	TMap<EMatchData, FMatchWidgetData> WidgetData;
};
