// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/GameStateMatchGame.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"
#include "OnlinetMatchGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewPlayerPostLogin, APlayerState*, PlayerState);

UCLASS()
class SPACEWAR_API AOnlinetMatchGameStateBase : public AGameStateMatchGame
{
	GENERATED_BODY()

	void SetTeamForPlayer(APlayerController* PC);

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_NewPlayerPostLogin(APlayerState* PlayerState);

	/** Call on game mode */
	int32 UpdateTeamPoints(ETeam Team, int32 Value);

public:

	AOnlinetMatchGameStateBase();

	UFUNCTION(BlueprintPure)
	int32 GetTeamPointsA() const { return TeamPointsA; }

	UFUNCTION(BlueprintPure)
	int32 GetTeamPointsB() const { return TeamPointsB; }

protected:

	virtual void NewPlayerLogin(APlayerController* PC) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Call if player log out */
	virtual void AutoBalanceTeam();

public:

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FNewPlayerPostLogin OnNewPlayerPostLogin;

private:

	UPROPERTY(Replicated)
	int32 TeamPointsA;

	UPROPERTY(Replicated)
	int32 TeamPointsB;

friend class AOnlineMatchGameModeBase;
};
