// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "SpecialObjectDataAsset.generated.h"

class ASpecialWeaponObjectBase;

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAsyncSpecialSpawnActor, bool, bResult, FStringAssetReference, LoadedRef, ASpecialWeaponObjectBase*, SpecialActor);

USTRUCT(BlueprintType)
struct FSpecialObject : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<ASpecialWeaponObjectBase> SpecialActor;

	FSpecialObject() :
		ObjectName(TEXT("None")),
		Price(0.f),
		Icon(nullptr),
		SpecialActor(nullptr) {}
};

UCLASS()
class SPACEWAR_API USpecialObjectDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "MyProject|MyAssetLibrary", meta = (WorldContext = "WorldContextObject", DisplayName = "Spawn Special object (Async)"))
	static bool AsyncSpawnActor(UObject* WorldContextObject, TAssetSubclassOf<ASpecialWeaponObjectBase> AssetPtr, FTransform SpawnTransform, APawn* Owner, const FAsyncSpecialSpawnActor& Callback);

private:

	/** Called when asset loading for actor spawn is finished */
	static void OnAsyncSpawnActorComplete(UObject* WorldContextObject, FStringAssetReference Reference, FTransform SpawnTransform, APawn* Owner, FAsyncSpecialSpawnActor Callback);
};
