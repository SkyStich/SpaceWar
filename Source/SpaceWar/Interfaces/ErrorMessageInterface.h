// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ErrorMessageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UErrorMessageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPACEWAR_API IErrorMessageInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Widget")
	void ClientErrorMessage(const FString& Message);
};
