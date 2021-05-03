// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TimeWorkLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UTimeWorkLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "TimeLibrary")
	static void ConvertSecondOnTimeStandard(int32 Seconds, FString& ReturnValue);
};
