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
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BuyIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DescriptionIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetSubclassOf<ASpecialWeaponObjectBase> SpecialActor;
};

UCLASS()
class SPACEWAR_API USpecialObjectDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "MyProject|MyAssetLibrary", meta = (WorldContext = "WorldContextObject", DisplayName = "Spawn Special object (Async)"))
	static bool AsyncSpawnActor(UObject* WorldContextObject, TAssetSubclassOf<ASpecialWeaponObjectBase> AssetPtr, FTransform SpawnTransform, class AMatchPlayerControllerBase* OwnerController, const FAsyncSpecialSpawnActor& Callback);

private:

	/** Called when asset loading for actor spawn is finished */
	static void OnAsyncSpawnActorComplete(UObject* WorldContextObject, FStringAssetReference Reference, FTransform SpawnTransform, class AMatchPlayerControllerBase* OwnerController, FAsyncSpecialSpawnActor Callback);
};
