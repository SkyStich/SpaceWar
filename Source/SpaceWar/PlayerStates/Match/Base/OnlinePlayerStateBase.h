// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SpaceWar/Enums/PlayerTeamEnum.h"
#include "SpaceWar/Interfaces/GetPlayerTeamInterface.h"
#include "OnlinePlayerStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNumberOfMurdersIncrease);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNumberOnMurdersDecrease);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNumberOfDeathsChanged);

UCLASS()
class SPACEWAR_API AOnlinePlayerStateBase : public APlayerState, public IGetPlayerTeamInterface
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_NumberOfMurders();
	
	UFUNCTION()
	void OnRep_NumberOfDeaths();

	UFUNCTION(Server, Reliable)
	void Server_TransferPlayerNameToServer(const FString& Name);

public:

	AOnlinePlayerStateBase();

	virtual ETeam FindPlayerTeam_Implementation() override { return PlayerTeam; }

	UFUNCTION(BlueprintPure, Category = "PlayerState|Statistics")
	int32 GetNumberOfMurders() const { return NumberOfMurders; }
	
	UFUNCTION(BlueprintPure, Category = "PlayerState|Statistics")
	int32 GetNumberOfDeaths() const { return NumberOfDeaths; }
	
	virtual void SetTeam(const ETeam NewTeam);

	void IncrementNumberOfDeaths();
	void IncrementNumberOfMurders();
	void DecrementNumberOfMurders();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay();

public:

	UPROPERTY(BlueprintAssignable, Category = "PlayerState|Delegates")
	FNumberOfMurdersIncrease OnNumberOfMurdersChanged;

	UPROPERTY(BlueprintAssignable, Category = "PlayerState|Delegates")
	FNumberOnMurdersDecrease OnNumberOnMurdersDecrease;
	
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
