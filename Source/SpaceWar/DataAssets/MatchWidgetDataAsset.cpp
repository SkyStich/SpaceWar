// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchWidgetDataAsset.h"

/*template<class T>
T* UMatchWidgetDataAsset::SyncCreateWidget(UObject* WorldContext, TSoftClassPtr<UUserWidget> WidgetClass, APlayerController* OwningPlayer) const
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
}*/
