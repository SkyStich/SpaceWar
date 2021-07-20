// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialObjectDataAsset.h"
#include "Engine/StreamableManager.h"
#include "SpaceWar/Actors/Match/SpecialWeapon/SpecialWeaponObjectBase.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "SpaceWar/Singleton/BaseSingleton.h"

bool USpecialObjectDataAsset::AsyncSpawnActor(UObject* WorldContextObject, TAssetSubclassOf<ASpecialWeaponObjectBase> AssetPtr, FTransform SpawnTransform, AMatchPlayerControllerBase* OwnerController, const FAsyncSpecialSpawnActor& Callback)
{
	if(AssetPtr == nullptr)
	{
		const FString InstigatorName = WorldContextObject ? *WorldContextObject->GetFullName() : TEXT("Uncknown");
		UE_LOG(LogAssetData, Error, TEXT("Asset class is NULL -- USpecialObjectDataAsset::AsyncSpawnActor"), *InstigatorName);
		return false;
	}
	FStreamableManager& StreamableManager = UBaseSingleton::Get().AssetLoader;
	FSoftObjectPath const Ref = AssetPtr.ToSoftObjectPath();

	StreamableManager.RequestAsyncLoad(Ref, FStreamableDelegate::CreateStatic(&USpecialObjectDataAsset::OnAsyncSpawnActorComplete, WorldContextObject, Ref, SpawnTransform, OwnerController, Callback));
	return true;
}

void USpecialObjectDataAsset::OnAsyncSpawnActorComplete(UObject* WorldContextObject, FStringAssetReference Reference, FTransform SpawnTransform, AMatchPlayerControllerBase* OwnerController, FAsyncSpecialSpawnActor Callback)
{
	ASpecialWeaponObjectBase* SpawnActor = nullptr;
	if(!OwnerController || OwnerController->GetCharacter()) return;

	UClass* ActorClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), OwnerController->GetPawn(), *Reference.ToString()));
	if(ActorClass == nullptr)
	{
		UE_LOG(LogAssetData, Error, TEXT("Actor class is NULL -- USpecialObjectDataAsset::OnAsyncSpawnActorComplete"), *Reference.ToString());
	}
	else
	{
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Param.Owner = OwnerController;
		Param.Instigator = OwnerController->GetPawn();
		SpawnActor = WorldContextObject->GetWorld()->SpawnActor<ASpecialWeaponObjectBase>(ActorClass, SpawnTransform, Param);
		SpawnActor->SetOwnerController(OwnerController);
	}
	Callback.ExecuteIfBound(SpawnActor != nullptr, Reference, SpawnActor);
}
