// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceWar/Components/SpecialObjectManagerComponent.h"
#include "SpaceWar/Interfaces/CustomInputInterface.h"
#include "SpaceWar/Interfaces/UpdateSpecialPointsInterface.h"
#include "SpaceWar/Interfaces/PlayerControllerInterface.h"
#include "MatchPlayerControllerBase.generated.h"

class ASpaceWarCharacter;

UCLASS(Blueprintable)
class SPACEWAR_API AMatchPlayerControllerBase : public APlayerController, public IUpdateSpecialPointsInterface, public IPlayerControllerInterface, public ICustomInputInterface
{
	GENERATED_BODY()

	void PressTabMenu();
	void ReleasedTabMenu();
	
	void PressSpecialShop();

	UFUNCTION(Client, Unreliable)
	void Client_SpecialObjectErrorSpawned();
	
public:

	AMatchPlayerControllerBase();

	void SetPlayerClass(TSubclassOf<ASpaceWarCharacter> NewPlayerClass);

	virtual void IncreaseSpecialPoint_Implementation(int32 const Value) override;
	virtual void DecreaseSpecialPoint_Implementation(int32 const Value) override;
	virtual bool SpawnPlayer(const FVector& Location);
	virtual bool IsPLayerCharacterAlive_Implementation() override { return GetCharacter() != nullptr; }

	/** Input Interface */
	virtual bool CheckKeyByName_Implementation(const FName Name) override;

	UFUNCTION(BlueprintPure, Category = "PlayerController|PlayerClass")
	TAssetSubclassOf<ASpaceWarCharacter> GetPlayerClass() const { return PlayerClass; }

	UFUNCTION(BlueprintPure, Category = "PlayerController|PlayerClass")
	USpecialObjectManagerComponent* GetSpecialManagerComponent() const { return SpecialObjectManager; }

	UFUNCTION(Server, Unreliable)
	void Server_CreateSpecialObject(const FName& ObjectId, const FTransform& Transform);

	UFUNCTION(BlueprintCallable, Category = "PlayerController|SpecialObject")
	bool OwnerAddSpecialObject(const FName& ObjectId);

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
	virtual void OnRep_Pawn() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

private:

	UPROPERTY(Replicated)
	TAssetSubclassOf<ASpaceWarCharacter>PlayerClass;

	UPROPERTY()
	USpecialObjectManagerComponent* SpecialObjectManager;

	UPROPERTY(EditAnywhere, Category = "Special")
	int32 DecreaseSpecialPointsValue;
};
