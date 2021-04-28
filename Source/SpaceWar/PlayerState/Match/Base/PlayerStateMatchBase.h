// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PlayerStateMatchBase.generated.h"

UCLASS()
class SPACEWAR_API APlayerStateMatchBase : public AGameStateBase
{
	GENERATED_BODY()

	UFUNCTION(NetMulticast, Reliable)
    void MatchFinish(const FString& Reason);

public:

	APlayerStateMatchBase();

	void IncrementTime();

	UFUNCTION(BlueprintPure, Category = "GameState|Getting")
	int32 GetCurrentMatchTime() const { return CurrentMatchTime; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Time")
	int32 MaxMatchTime;
	
	UPROPERTY(Replicated)
	int32 CurrentMatchTime;
};
