// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ServerManipulationLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UServerManipulationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "BPFLibrary|ServerManipulation")
	static void LaunchServer(const FString& MapAddress, const FString& Attributes);
};
