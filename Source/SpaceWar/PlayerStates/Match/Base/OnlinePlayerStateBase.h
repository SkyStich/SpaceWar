// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OnlinePlayerStateBase.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	NoneTeam,
	TeamA,
    TeamB
};

UCLASS()
class SPACEWAR_API AOnlinePlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "PlayerState|Team")
	ETeam GetPlayerTeam() const { return PlayerTeam; }
	
	virtual void SetTeam(const ETeam NewTeam);

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(Replicated)
	ETeam PlayerTeam;
};
