// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpaceWar/Enums/PlayerTeamEnum.h"
#include "GameStateMatchGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMatchEnd, const FString&, Reason, ETeam, WinnerTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEndPreMatch, const FString&, Reason, ETeam, TeamWinner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayerDeath, const FString&, InstigatorName, const FString&, LoserName, AActor*, DamageCauser);

UCLASS()
class SPACEWAR_API AGameStateMatchGame : public AGameStateBase
{
	GENERATED_BODY()

private:

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_OnPlayerDead(const FString& InstigatorName, const FString& LoserName, AActor* DamageCauser);

public:
    
    AGameStateMatchGame();
    
    virtual void IncrementTime();

	UFUNCTION(BlueprintPure, Category = "GameState|Getting")
    int32 GetCurrentMatchTime() const { return CurrentMatchTime; }
    
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
    
    virtual void BeginPlay() override;

	UFUNCTION()
	virtual void NewPlayerLogin(APlayerController* PC);

	UFUNCTION()
	virtual void PlayerDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser);
		
	UFUNCTION(NetMulticast, Reliable)
    void MatchFinish(const FString& Reason, ETeam WinnerTeam);

	UFUNCTION(NetMulticast, Reliable)
	void PreMatchFinish(const FString& Reason, ETeam WinnerTeam);

protected:

	UPROPERTY(EditAnywhere, Category = "Time")
	int32 MaxMatchTime;
    	
	UPROPERTY(Replicated)
	int32 CurrentMatchTime;

public:

	UPROPERTY(BlueprintAssignable, Category = "GameState|Delegate")
	FMatchEnd OnMatchEnd;

	UPROPERTY(BlueprintAssignable, Category = "GameState|Delegate")
	FEndPreMatch OnPreMatchEnd;
	
	UPROPERTY(BlueprintAssignable, Category = "GameState|Delegate")
	FPlayerDeath OnPlayerDead;
};