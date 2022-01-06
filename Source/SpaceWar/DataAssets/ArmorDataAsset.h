// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArmorDataAsset.generated.h"

class UBaseArmorObject;

USTRUCT(BlueprintType)
struct FArmorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	FName DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	bool bCanUseJetPack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	float MaxArmor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	float ArmorRegenerationPerSec; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	float MaxStaminaSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	float MaxBaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	float JumpLenght;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	float MaxWeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArmorData")
	TAssetSubclassOf<UBaseArmorObject> ArmorClass;
};

UCLASS(BlueprintType)
class SPACEWAR_API UArmorDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	static UBaseArmorObject* SyncCreateArmorObject(UObject* WorldContext, FArmorData Data, class ASpaceWarCharacter* Outer);
	FArmorData FindData(const FName& Id) const { return ArmorDataMap.FindRef(Id); }
	bool CheckId(const FName& Id) const;
	
	UFUNCTION(BlueprintPure)
	TMap<FName, FArmorData> GetArmorData() const { return ArmorDataMap; }

private:

	UPROPERTY(EditAnywhere, Category = "Armor")
	TMap<FName, FArmorData> ArmorDataMap;
};
