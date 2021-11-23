// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"
#include "SpecialControllerBase.generated.h"

UCLASS()
class SPACEWAR_API ASpecialControllerBase : public AAIController
{
	GENERATED_BODY()

protected:

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
public:

	ASpecialControllerBase();
	
};
