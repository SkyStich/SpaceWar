// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GetDamageCauserInfo.generated.h"

USTRUCT(BlueprintType)
struct FDamageCauserInfo
{
	GENERATED_BODY()

	FDamageCauserInfo() : CauserName("None"), Image(nullptr) {}

	FDamageCauserInfo(const FName& NewCauserName, UTexture2D* NewImage) : CauserName(NewCauserName), Image(NewImage) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CauserName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGetDamageCauserInfo : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class SPACEWAR_API IGetDamageCauserInfo
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetCauserInfo(FDamageCauserInfo& DamageCauserInfo);
};
