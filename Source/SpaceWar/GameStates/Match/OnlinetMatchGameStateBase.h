// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/GameStateMatchGame.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"
#include "OnlinetMatchGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewPlayerPostLogin, APlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTeamPointUpdate, int32, NewValue, ETeam, TeamUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLaunchTimerBeforeOfGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreparationStartGameFinish, bool, bResult /** if true, game start else start wait new player*/);

UCLASS()
class SPACEWAR_API AOnlinetMatchGameStateBase : public AGameStateMatchGame
{
	GENERATED_BODY()

	void SetTeamForPlayer(APlayerController* PC);

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_NewPlayerPostLogin(APlayerState* PlayerState);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_FinishPreparationStartGame(bool bResult);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_StartTimerBeforeOfGame();

public:

	AOnlinetMatchGameStateBase();

	UFUNCTION(BlueprintPure)
	int32 GetTeamPointsA() const { return TeamPointsA; }

	UFUNCTION(BlueprintPure)
	int32 GetTeamPointsB() const { return TeamPointsB; }
	
	virtual void UpdateTeamPoints(ETeam Team, int32 Value);

protected:

	virtual void NewPlayerLogin(APlayerController* PC) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Call if player log out */
	virtual void AutoBalanceTeam();
	
	UFUNCTION()
    virtual void FinishPreparationGame(bool bResult);
    
    UFUNCTION()
    virtual void StartTimerBeforeGame();

public:

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FNewPlayerPostLogin OnNewPlayerPostLogin;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FTeamPointUpdate OnTeamPointUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FLaunchTimerBeforeOfGame OnLaunchTimerBeforeOfGame;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FPreparationStartGameFinish OnPreparationStartGameFinish;

protected:

	UPROPERTY(Replicated)
	int32 TeamPointsA;

	UPROPERTY(Replicated)
	int32 TeamPointsB;

friend class AOnlineMatchGameModeBase;
};
