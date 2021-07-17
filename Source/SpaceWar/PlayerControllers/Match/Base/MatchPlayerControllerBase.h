// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceWar/Components/SpecialObjectManagerComponent.h"
#include "SpaceWar/Interfaces/MatchChatInterface.h"
#include "SpaceWar/Components/ChatMatchComponent.h"
#include "SpaceWar/Interfaces/CustomInputInterface.h"
#include "SpaceWar/Interfaces/UpdateSpecialPointsInterface.h"
#include "SpaceWar/Interfaces/PlayerControllerInterface.h"
#include "MatchPlayerControllerBase.generated.h"

class ASpaceWarCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetMessage, const FString&, Message, bool, IsOnlyAlly);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPausePressed);

UCLASS(Blueprintable)
class SPACEWAR_API AMatchPlayerControllerBase : public APlayerController, public IUpdateSpecialPointsInterface, public IPlayerControllerInterface, public ICustomInputInterface, public IMatchChatInterface
{
	GENERATED_BODY()

	void PressTabMenu();
	void ReleasedTabMenu();
	
	void PressSpecialShop();

	void CreateChatComponent();

	UFUNCTION(Client, Unreliable)
	void Client_SpecialObjectErrorSpawned();

	UFUNCTION(Client, Reliable)
	void Client_GetMessage(const FString& Message, bool const IsOnlyAlly);

	UFUNCTION()
	void CreateChat();

	UFUNCTION()
	void ToggleMainCharacterHUD();
	
	UFUNCTION()
	void PausePressed();
	
public:

	AMatchPlayerControllerBase();

	void SetPlayerClass(TSubclassOf<ASpaceWarCharacter> NewPlayerClass);

	virtual void IncreaseSpecialPoint_Implementation(int32 const Value) override;
	virtual void DecreaseSpecialPoint_Implementation(int32 const Value) override;
	virtual bool SpawnPlayer(const FVector& Location);
	virtual bool IsPLayerCharacterAlive_Implementation() override { return GetCharacter() != nullptr; }

	/** Input Interface */
	virtual bool CheckKeyByName_Implementation(const FName Name) override;

	/** chat interface */
	virtual void GetMessage_Implementation(const FString& Message, bool const IsOnlyAlly) override;

	UFUNCTION(BlueprintPure, Category = "PlayerController|PlayerClass")
	TAssetSubclassOf<ASpaceWarCharacter> GetPlayerClass() const { return PlayerClass; }

	UFUNCTION(BlueprintPure, Category = "PlayerController|PlayerClass")
	USpecialObjectManagerComponent* GetSpecialManagerComponent() const { return SpecialObjectManager; }

	UFUNCTION(Server, Unreliable)
	void Server_CreateSpecialObject(const FName& ObjectId, const FTransform& Transform);

	UFUNCTION(BlueprintCallable, Category = "PlayerController|SpecialObject")
	bool OwnerAddSpecialObject(const FName& ObjectId);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PlayerController|Chat")
	void Server_SendMessageFromAllPlayers(const FString& Message);

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
	virtual void OnRep_Pawn() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable, Category = "PlayerController|Delegate")
	FGetMessage OnGetMessage;
	
	UPROPERTY(BlueprintAssignable, Category = "PlayerController|Delegate")
	FPausePressed OnPausePressed;

private:

	UPROPERTY(Replicated)
	TAssetSubclassOf<ASpaceWarCharacter>PlayerClass;

	UPROPERTY()
	USpecialObjectManagerComponent* SpecialObjectManager;

	UPROPERTY(EditAnywhere, Category = "Special")
	int32 DecreaseSpecialPointsValue;

	UPROPERTY()
	UChatMatchComponent* ChatComponent;
};
