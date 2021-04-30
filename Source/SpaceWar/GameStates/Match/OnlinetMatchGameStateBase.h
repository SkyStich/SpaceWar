// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/GameStateMatchGame.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"
#include "OnlinetMatchGameStateBase.generated.h"

UCLASS()
class SPACEWAR_API AOnlinetMatchGameStateBase : public AGameStateMatchGame
{
	GENERATED_BODY()

	void SetTeamForPlayer(APlayerController* PC);

public:

	AOnlinetMatchGameStateBase();

	void UpdateTeamPoints(ETeam Team, int32 Value);

	UFUNCTION(BlueprintPure)
	int32 GetTeamPointsA() const { return TeamPointsA; }

	UFUNCTION(BlueprintPure)
	int32 GetTeamPointsB() const { return TeamPointsB; }

protected:

	virtual void NewPlayerLogin(APlayerController* PC) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Call if player log out */
	virtual void AutoBalanceTeam();

private:

	UPROPERTY(Replicated)
	int32 TeamPointsA;

	UPROPERTY(Replicated)
	int32 TeamPointsB;
};
