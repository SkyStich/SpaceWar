// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadObjectsInMemoryLibrary.h"
#include "SpaceWar/Singleton/BaseSingleton.h"

UTexture2D* ULoadObjectsInMemoryLibrary::SyncLoadIcon(UObject* WorldContext, TAssetPtr<UTexture2D> IconPtr)
{
	if(IconPtr.IsNull())
	{
		FString const InstigatorName = WorldContext ? WorldContext->GetFullName() : TEXT("uncknown");
		UE_LOG(LogTemp, Error, TEXT("Icon ptr is null --ULoadObjectsInMemoryLibrary::SyncLoadIcon"), *InstigatorName);
		return nullptr;
	}
	FSoftObjectPath const ObjectPtr = IconPtr.ToSoftObjectPath();
	IconPtr = Cast<UTexture2D>(UBaseSingleton::Get().AssetLoader.LoadSynchronous(ObjectPtr));
	return IconPtr.Get();
}
