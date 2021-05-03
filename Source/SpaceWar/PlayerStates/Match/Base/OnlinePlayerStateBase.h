// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OnlinePlayerStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNumberOfMurdersChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNumberOfDeathsChanged);

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

	UFUNCTION()
	void OnRep_NumberOfMurders();
	
	UFUNCTION()
	void OnRep_NumberOfDeaths();

public:

	UFUNCTION(BlueprintPure, Category = "PlayerState|Team")
	ETeam GetPlayerTeam() const { return PlayerTeam; }

	UFUNCTION(BlueprintPure, Category = "PlayerState|Statistics")
	int32 GetNumberOfMurders() const { return NumberOfMurders; }
	
	UFUNCTION(BlueprintPure, Category = "PlayerState|Statistics")
	int32 GetNumberOfDeaths() const { return NumberOfDeaths; }
	
	virtual void SetTeam(const ETeam NewTeam);

	void IncrementNumberOfDeaths();
	//void DecrementNumberOfDeaths();
	void IncrementNumberOfMurders();
	void DecrementNumberOfMurders();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UPROPERTY(BlueprintAssignable, Category = "PlayerState|Delegates")
	FNumberOfMurdersChanged OnNumberOfMurdersChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "PlayerState|Delegates")
	FNumberOfDeathsChanged OnNumberOfDeathsChanged;
	
private:

	UPROPERTY(Replicated)
	ETeam PlayerTeam;

	UPROPERTY(ReplicatedUsing = OnRep_NumberOfMurders)
	int32 NumberOfMurders;

	UPROPERTY(ReplicatedUsing = OnRep_NumberOfDeaths)
	int32 NumberOfDeaths;
};
