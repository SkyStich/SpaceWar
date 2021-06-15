// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../OnlineMatchGameModeBase.h"
#include "CaptureHoldGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ACaptureHoldGameMode : public AOnlineMatchGameModeBase
{
	GENERATED_BODY()

public:

	ACaptureHoldGameMode();

protected:

	virtual void CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser) override;
	
};
