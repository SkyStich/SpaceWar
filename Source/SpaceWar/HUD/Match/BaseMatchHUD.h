// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpaceWar/UserWidget/ErrorWidget/ErrorMessageWidget.h"
#include "SpaceWar/UserWidget/Match/AmmunitionWidgetBase.h"
#include "SpaceWar/Interfaces/ErrorMessageInterface.h"
#include "SpaceWar/UserWidget/Match/EndGameWidgetBase.h"
#include "SpaceWar/DataAssets/MatchWidgetDataAsset.h"
#include "SpaceWar/UserWidget/Match/MatchChatWidgetBase.h"
#include "BaseMatchHUD.generated.h"

UCLASS(BlueprintType)
class SPACEWAR_API ABaseMatchHUD : public AHUD, public IErrorMessageInterface
{
	GENERATED_BODY()

	UFUNCTION()
	UErrorMessageWidget* CreateErrorWidget(const FString& Message);
	
	UFUNCTION()
	void OnPausePressed();

protected:
	
	UFUNCTION()
	virtual void OnPreparationStartGameEvent(bool bResult) {}

	virtual void BeginPlay() override;
	virtual void NewOwningPlayerPawn(APawn* NewPawn);
	
	virtual void CreateCharacterWidgets();
	virtual void RemoveCharacterWidgets();

	virtual void CreateKillMessage();
	virtual void RemoveKillMessage();
	
	virtual void CreateSpectatorWidgets();
	virtual void RemoveSpectatorWidgets();
	
	virtual void CreatePreparationWidget();
	virtual void RemovePreparationWidget();
	
	virtual void CreateSpecialWidget();
	virtual void RemoveSpecialWidget();
	
	virtual void CreatePreMatchEnd(const FString& Reason, ETeam WinnerTeam);
	virtual void RemovePreMatchEnd();
	
	virtual void CreatePauseMenu();
	virtual void RemovePauseMenu();

	virtual void CreateChatWidget();
	
	virtual void CreateTabMenu();

	virtual UErrorMessageWidget* ClientErrorMessage_Implementation(const FString& Message) override;
	
	UFUNCTION()
	virtual void CreateMatchEndWidget(const FString& Reason, ETeam WinnerTeam);

	UFUNCTION()
    virtual void RemoveMatchEndWidget();

public:

	ABaseMatchHUD();
	
	UFUNCTION(BlueprintPure)
	UUserWidget* GetMainWidget() const { return CharacterHUD; }

	UFUNCTION()
	void ShowMainCharacterWidget();

	UFUNCTION()
	void HiddenMainCharacterWidget();

	UFUNCTION()
	void CreateAmmunitionWidget();

	UFUNCTION()
	void RemoveAmmunitionWidget();
	
	UFUNCTION()
	void ShowChatForAllPlayers();
	
	void ShowChatForAnAlly();
	void HiddenChat();
	 
	void ShowTabMenu();
	void HiddenTabMenu();

	virtual void ShowSpecialWidget();

	UUserWidget* GetSpecialShop() const { return SpecialShopWidget; }

protected:
	
	FMatchWidgetData* MatchWidgetData;
	EMatchData MatchType;

	UPROPERTY()
	TAssetSubclassOf<UErrorMessageWidget> ErrorWidgetClass;

	UPROPERTY()
	TAssetSubclassOf<UMatchChatWidgetBase> ChatWidgetClass;

	UPROPERTY()
	TAssetSubclassOf<UAmmunitionWidgetBase> AmmunitionWidgetClass;

	UPROPERTY()
	TAssetSubclassOf<UUserWidget> SpecialShopWidgetClass;

	UPROPERTY()
	TAssetSubclassOf<UUserWidget> KillMessageClass;
	
	UPROPERTY()
	TAssetSubclassOf<UUserWidget> PauseWidgetClass;
	
	UPROPERTY()
	TAssetSubclassOf<UUserWidget> SpectatorWidget;

	UPROPERTY()
	UMatchWidgetDataAsset* AssetData;

	UPROPERTY()
	UUserWidget* CharacterHUD;
	
	UPROPERTY()
	UUserWidget* SpectatorHUD;

	UPROPERTY()
	UUserWidget* TabMenuWidget;

	UPROPERTY()
	UUserWidget* SpecialShopWidget;

	UPROPERTY()
	UUserWidget* PreparationWidget;

	UPROPERTY()
	UUserWidget* KillMessageWidget;

	UPROPERTY()
	UUserWidget* PauseWidget;

	UPROPERTY()
	UEndGameWidgetBase* EndGameMatchWidget;
	
	UPROPERTY()
	UEndGameWidgetBase* PreEndMatchWidget;

	UPROPERTY()
	UMatchChatWidgetBase* ChatWidget;

	UPROPERTY()
	UAmmunitionWidgetBase* AmmunitionWidget;
};
