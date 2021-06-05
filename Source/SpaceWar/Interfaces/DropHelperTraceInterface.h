// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DropHelperTraceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDropHelperTraceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPACEWAR_API IDropHelperTraceInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void DropHelperTrace(FVector& Location, const FVector& PlayerDirection, float const Range);
};
