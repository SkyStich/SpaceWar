// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlinetMatchGameStateBase.h"
#include "CaptureOfFlagGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoundEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoundStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoundPreparation);

UCLASS()
class SPACEWAR_API ACaptureOfFlagGameState : public AOnlinetMatchGameStateBase
{
	GENERATED_BODY()

public:

	ACaptureOfFlagGameState();
	virtual void IncrementTime() override;

protected:

	virtual int32 UpdateTeamPoints(ETeam Team, int32 Value) override;

	virtual void RefreshRound();
	virtual void MatchFinish_Implementation(const FString& Reason) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PlayerDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser) override;

	UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_RoundEnded();

	UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_RoundStarted();

	UFUNCTION(NetMulticast, Reliable)
    void NetMulticast_RoundPreparation();

	UFUNCTION(BlueprintPure)
    int32 GetCurrentPreparationTime() const { return CurrentPreparationTime; }

	UFUNCTION(BlueprintPure)
	ETeam GetSecurityTeam() const { return SecurityTeam; }
    
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

	int32 CurrentPreparationTime;

	UPROPERTY(Replicated)
	ETeam SecurityTeam;
};
