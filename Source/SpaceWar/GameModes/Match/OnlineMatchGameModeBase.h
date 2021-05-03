// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/MatchGameModeBase.h"
#include "SpaceWar/GameStates/Match/OnlinetMatchGameStateBase.h"
#include "OnlineMatchGameModeBase.generated.h"

UCLASS()
class SPACEWAR_API AOnlineMatchGameModeBase : public AMatchGameModeBase
{
	GENERATED_BODY()

	void UpdatePlayerStatistics(AController* InstigatorController, AController* LoserController);

public:

	void UpdateTeamPoints(const int32 Value, ETeam Team);
	virtual void CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser) override;
	
};