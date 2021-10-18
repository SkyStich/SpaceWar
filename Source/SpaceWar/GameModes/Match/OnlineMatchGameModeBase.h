// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/MatchGameModeBase.h"
#include "SpaceWar/GameStates/Match/OnlinetMatchGameStateBase.h"
#include "OnlineMatchGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerLogout, AController*, Exiting);

/** Game mode with Team logic */
UCLASS(Abstract)
class SPACEWAR_API AOnlineMatchGameModeBase : public AMatchGameModeBase
{
	GENERATED_BODY()

	void UpdatePlayerStatistics(AController* InstigatorController, AController* LoserController);

	UFUNCTION()
	void PreparationGameFinish(bool bResult);

protected:

	virtual void TickTime(AOnlinetMatchGameStateBase* MatchPlayerState);
	virtual void BeginPlay() override;
	virtual void Logout(AController* Exiting) override;

	UFUNCTION()
	virtual void MatchStarted() {}
	
	UFUNCTION()
    virtual void UpdateTeamPoints(int32 NewValue, ETeam Team);

public:

	AOnlineMatchGameModeBase();

	virtual void LaunchGameTimer();
	virtual void CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser) override;

public:

	UPROPERTY()
	FPlayerLogout OnPlayerLogout;
};
