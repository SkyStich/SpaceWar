// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchWidgetDataAsset.h"
#include "SpaceWar/Singleton/BaseSingleton.h"
#include "Blueprint/UserWidget.h"

UUserWidget* UMatchWidgetDataAsset::SyncCreateWidget(UObject* WorldContext, TSoftClassPtr<UUserWidget> WidgetClass, APlayerController* OwningPlayer)
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

	UUserWidget* Widget = nullptr;
	if(OwningPlayer == nullptr)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
		Widget = CreateWidget<UUserWidget>(World, WidgetType);
	}
	else
	{
		Widget = CreateWidget<UUserWidget>(OwningPlayer, WidgetType);
	}
	
	if(Widget)
	{
		Widget->SetFlags(EObjectFlags::RF_StrongRefOnFrame);
	}
	
	return Widget;
}
