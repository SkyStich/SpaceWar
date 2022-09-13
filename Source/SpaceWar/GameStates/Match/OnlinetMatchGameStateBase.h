// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/GameStateMatchGame.h"
#include "SpaceWar/Interfaces/MainGameStateInterface.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"
#include "OnlinetMatchGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewPlayerPostLogin, APlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTeamPointUpdate, int32, NewValue, ETeam, TeamUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreparationStartGameFinish, bool, bResult /** if true, game start else start wait new player*/);

USTRUCT()
struct FVotingData
{

	GENERATED_BODY()
	
	FVotingData() : VotingTimer(FTimerHandle()), KickPlayer(nullptr), AmountForKick(0.f), AmountForSave(0.f) {}

	UPROPERTY()
	FTimerHandle VotingTimer;

	UPROPERTY()
	APlayerController* KickPlayer;

	UPROPERTY()
	int32 AmountForKick;

	UPROPERTY()
	int32 AmountForSave;
};

UCLASS()
class SPACEWAR_API AOnlinetMatchGameStateBase : public AGameStateMatchGame, public IMainGameStateInterface
{
	GENERATED_BODY()

	void SetTeamForPlayer(APlayerController* PC);
	void PreparationForStartGame();

	UFUNCTION()
	void Logout(AController* Exiting);

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_NewPlayerPostLogin(APlayerState* PlayerState);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_FinishPreparationStartGame(bool bResult);

	void StopVoting(FVotingData* VotingData);

	FTimerHandle* GetVotingTimerByTeam(ETeam const Team);
	FVotingData* GetVotingDataByTeam(ETeam const Team);

public:

	AOnlinetMatchGameStateBase();

	UFUNCTION(BlueprintPure)
	int32 GetTeamPointsA() const { return TeamPointsA; }

	UFUNCTION(BlueprintPure)
	int32 GetTeamPointsB() const { return TeamPointsB; }
	
	UFUNCTION(BlueprintPure)
	FTimerHandle GetPreparationGameStartHandle() const { return PreparationGameStartHandle; }

	UFUNCTION(BlueprintPure)
	bool GameInProgress() const { return bGameInProgress; }

	void StartVotingForKickPlayer(APlayerController* KickPlayer);

	virtual void StartVotingForKickPlayer_Implementation(APlayerController* Player, ETeam const VotingOwnerTeam) override;
	
	virtual void UpdateTeamPoints(ETeam Team, int32 Value);

protected:

	virtual void NewPlayerLogin(APlayerController* PC) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Call if player log out */
	virtual void AutoBalanceTeam();

	virtual void MatchStarted();
	
	UFUNCTION()
    void FinishPreparationGame(bool bResult);

public:

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FNewPlayerPostLogin OnNewPlayerPostLogin;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FTeamPointUpdate OnTeamPointUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FPreparationStartGameFinish OnPreparationStartGameFinish;

protected:

	UPROPERTY(Replicated)
	int32 TeamPointsA;

	UPROPERTY(Replicated)
	int32 TeamPointsB;
	
private:

	UPROPERTY()
	FTimerHandle PreparationGameStartHandle;

	UPROPERTY(Replicated)
	bool bGameInProgress;

	UPROPERTY()
	TArray<FVotingData> VotingDatas;
	
friend class AOnlineMatchGameModeBase;
};
