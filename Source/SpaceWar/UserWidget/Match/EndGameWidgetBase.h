// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceWar/Enums/PlayerTeamEnum.h"
#include "EndGameWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UEndGameWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void Init(const FString& Reason, ETeam Winner);
	
};
