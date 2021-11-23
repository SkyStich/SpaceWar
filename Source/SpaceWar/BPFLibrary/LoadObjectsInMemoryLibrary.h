// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LoadObjectsInMemoryLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ULoadObjectsInMemoryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, meta=(DisplayName="BP_SyncLoadIcon"))
	static UTexture2D* SyncLoadIcon(UObject* WorldContext, TAssetPtr<UTexture2D> IconPtr); 
};
