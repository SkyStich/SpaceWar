// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceWar/GameStates/Match/OnlinetMatchGameStateBase.h"

#include "KickPlayerBaseWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SPACEWAR_API UKickPlayerBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void KickPlayer(APlayerController* Player);
};
