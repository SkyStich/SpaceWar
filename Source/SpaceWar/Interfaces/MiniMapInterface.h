// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SpaceWar/UserWidget/Match/MiniMapBase.h"
#include "UObject/Interface.h"
#include "MiniMapInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMiniMapInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPACEWAR_API IMiniMapInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interfaces/MiniMap")
	UMiniMapBase* GetMiniMapWidget();
};
