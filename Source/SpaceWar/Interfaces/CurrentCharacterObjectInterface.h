// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CurrentCharacterObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCurrentCharacterObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPACEWAR_API ICurrentCharacterObjectInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces")
	bool UseCurrentPlayerObject();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces")
	void UnUseCurrentPlayerObject();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintAuthorityOnly, Category = "Interfaces")
	bool InteractionObject(class ASpaceWarCharacter* Player);
};
