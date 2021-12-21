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
class UExperienceComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetMessage, const FString&, Message, bool, IsOnlyAlly);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FFindPlayerLevel, int32 , Level, int32, Exp);
DECLARE_DYNAMIC_DELEGATE(FPausePressed);

UCLASS(Blueprintable)
class SPACEWAR_API AMatchPlayerControllerBase : public APlayerController, public IUpdateSpecialPointsInterface,
	public IPlayerControllerInterface, public ICustomInputInterface, public IMatchChatInterface
{
	GENERATED_BODY()

	void PressTabMenu();
	void ReleasedTabMenu();
	
	void PressSpecialShop();

	void CreateChatComponent();

	void UpdateExp(const int32 Value);
	void ReceivingUpdateLevel(const int32 Exp);

	UFUNCTION(Client, Unreliable)
	void Client_SpecialObjectErrorSpawned();

	UFUNCTION(Client, Reliable)
	void Client_GetMessage(const FString& Message, bool const IsOnlyAlly);

	UFUNCTION()
	void CreateChatForAllPlayer();

	UFUNCTION()
	void CreateChatForAnAlly();

	UFUNCTION()
	void ToggleMainCharacterHUD();
	
	UFUNCTION()
	void PausePressed();
	
	UFUNCTION()
	void EndMatch(const FString& Reason, ETeam WinnerTeam);

	UFUNCTION()
	void PreEndMatch(const FString& Reason, ETeam WinnerTeam);

	UFUNCTION()
	void ForcedDisconnectFromServer();
	
	UFUNCTION(Client, Reliable)
	void Client_ConnectToHUBServer();

	UFUNCTION(Client, Reliable)
	void Client_UpdateLevelInfo(int32 Exp);
	
public:

	AMatchPlayerControllerBase(const FObjectInitializer& ObjectInitializer);

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
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "PlayerController|Chat")
	void Server_SendMessageFromAnAlly(const FString& Message);

	UFUNCTION(Client, Unreliable)
	void UseAmmunitionState();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void StopUseAmmunitionState();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
	virtual void OnRep_Pawn() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable, Category = "PlayerController|Delegate")
	FGetMessage OnGetMessage;
	
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

	UPROPERTY()
	class UDataBaseTransfer* DataBaseTransfer;
};
