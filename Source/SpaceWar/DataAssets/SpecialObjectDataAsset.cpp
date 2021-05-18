// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialObjectDataAsset.h"
#include "Engine/StreamableManager.h"
#include "SpaceWar/Singleton/BaseSingleton.h"

bool USpecialObjectDataAsset::AsyncSpawnActor(UObject* WorldContextObject, TAssetSubclassOf<ASpecialWeaponObjectBase> AssetPtr, FTransform SpawnTransform, const FAsyncSpecialSpawnActor& Callback)
{
	if(AssetPtr == nullptr)
	{
		const FString InstigatorName = WorldContextObject ? *WorldContextObject->GetFullName() : TEXT("Uncknown");
		UE_LOG(LogAssetData, Error, TEXT("Asset class is NULL -- USpecialObjectDataAsset::AsyncSpawnActor"), *InstigatorName);
		return false;
	}
	FStreamableManager& StreamableManager = UBaseSingleton::Get().AssetLoader;
	FSoftObjectPath const Ref = AssetPtr.ToSoftObjectPath();

	StreamableManager.RequestAsyncLoad(Ref, FStreamableDelegate::CreateStatic(&USpecialObjectDataAsset::OnAsyncSpawnActorComplete, WorldContextObject, Ref, SpawnTransform, Callback));
	return true;
}

void USpecialObjectDataAsset::OnAsyncSpawnActorComplete(UObject* WorldContextObject, FStringAssetReference Reference, FTransform SpawnTransform, FAsyncSpecialSpawnActor Callback)
{
	ASpecialWeaponObjectBase* SpawnActor = nullptr;

	UClass* ActorClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *Reference.ToString()));
	if(ActorClass == nullptr)
	{
		UE_LOG(LogAssetData, Error, TEXT("Actor class is NULL -- USpecialObjectDataAsset::OnAsyncSpawnActorComplete"), *Reference.ToString());
	}
	else
	{
		SpawnActor = WorldContextObject->GetWorld()->SpawnActor<ASpecialWeaponObjectBase>(ActorClass, SpawnTransform);
	}
	Callback.ExecuteIfBound(SpawnActor != nullptr, Reference, SpawnActor);
}
