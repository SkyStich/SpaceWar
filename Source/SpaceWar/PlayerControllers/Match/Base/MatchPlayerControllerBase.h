// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceWar/Components/SpecialObjectManagerComponent.h"

#include "MatchPlayerControllerBase.generated.h"

class ASpaceWarCharacter;

UCLASS(Blueprintable)
class SPACEWAR_API AMatchPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
	UFUNCTION(Server, Unreliable)
	void Server_SpawnPlayerPressed();

	void PressTabMenu();
	void ReleasedTabMenu();

	UFUNCTION(Client, Unreliable)
	void Client_SpecialObjectErrorSpawned();
	
public:

	AMatchPlayerControllerBase();

	void SetPlayerClass(TSubclassOf<ASpaceWarCharacter> NewPlayerClass);

	void SpawnPlayer();
	void LaunchRespawnTimer(float const Time);

	UFUNCTION(BlueprintPure, Category = "PlayerController|PlayerClass")
	TSubclassOf<ASpaceWarCharacter> GetPlayerClass() const { return PlayerClass; }

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Server_CreateSpecialObject(const FName& ObjectId, const FTransform& Transform);

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
	virtual void OnRep_Pawn() override;

private:

	UPROPERTY(Replicated)
	TSubclassOf<ASpaceWarCharacter>PlayerClass;
	
	UPROPERTY(Replicated)
	bool bCanSpawn;

	UPROPERTY()
	USpecialObjectManagerComponent* SpecialObjectManager;

	FTimerHandle RespawnTimer;
};
