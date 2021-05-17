// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "SpecialObjectDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FSpecialObject : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Price;
};

UCLASS()
class SPACEWAR_API USpecialObjectDataAsset : public UDataAsset
{
	GENERATED_BODY()

};
