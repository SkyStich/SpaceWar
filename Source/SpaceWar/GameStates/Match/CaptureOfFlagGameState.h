// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlinetMatchGameStateBase.h"
#include "CaptureOfFlagGameState.generated.h"

/** need for sound */
UENUM(BlueprintType)
enum class EReasonForEndOfRound : uint8
{
	AllEnemyDeath,
    FlagBeCapture,
	FlagBeSaved
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRoundEnded, const FString&, Reason, ETeam, WinnerTeam, EReasonForEndOfRound, ReasonEndOfRound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoundStarted, ETeam, SecurityTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoundPreparation);

UCLASS()
class SPACEWAR_API ACaptureOfFlagGameState : public AOnlinetMatchGameStateBase
{
	GENERATED_BODY()

public:

	ACaptureOfFlagGameState();
	
	virtual void IncrementTime() override;
	virtual void UpdateTeamPoints(ETeam Team, int32 Value) override { Super::UpdateTeamPoints(Team, Value); }
	virtual void MatchStarted() override;
	void UpdateTeamPoints(ETeam Team, int32 Value, EReasonForEndOfRound ReasonEndOfRound);

	UFUNCTION(BlueprintPure)
	ETeam GetSecurityTeam() const { return SecurityTeam; }

protected:

	virtual void RefreshRound();
	virtual void MatchFinish_Implementation(const FString& Reason, ETeam WinnerTeam) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PlayerDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser) override;

	UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_RoundEnded(const FString& Reason, ETeam WinnerTeam, EReasonForEndOfRound ReasonEndOfRound);

	UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_RoundStarted();

	UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_RoundPreparation();

	UFUNCTION(BlueprintPure)
    int32 GetCurrentPreparationTime() const { return CurrentPreparationTime; }
    
private:
	
	void ReductionPreparationTime();

public:

	UPROPERTY(BlueprintAssignable)
	FRoundEnded OnRoundEnded;

	UPROPERTY(BlueprintAssignable)
	FRoundStarted OnRoundStarted;
	
	UPROPERTY(BlueprintAssignable)
	FRoundPreparation OnRoundPreparation;

private:

	FTimerHandle RefreshRoundHandle;

protected:

	UPROPERTY(Replicated)
	int32 CurrentPreparationTime;

	UPROPERTY(Replicated)
	ETeam SecurityTeam;
};
