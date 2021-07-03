// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpaceWar/UserWidget/ErrorWidget/ErrorMessageWidget.h"
#include "SpaceWar/Interfaces/ErrorMessageInterface.h"
#include "SpaceWar/UserWidget/Match/EndGameWidgetBase.h"
#include "SpaceWar/DataAssets/MatchWidgetDataAsset.h"
#include "BaseMatchHUD.generated.h"

UCLASS(BlueprintType)
class SPACEWAR_API ABaseMatchHUD : public AHUD, public IErrorMessageInterface
{
	GENERATED_BODY()

	UFUNCTION()
	void CreateErrorWidget(const FString& Message);

protected:

	virtual void BeginPlay() override;
	virtual void NewOwningPlayerPawn(APawn* NewPawn);
	
	virtual void CreateCharacterWidgets();
	virtual void RemoveCharacterWidgets();
	
	virtual void CreateSpectatorWidgets();
	virtual void RemoveSpectatorWidgets();
	
	virtual void CreatePreparationWidget();
	virtual void RemovePreparationWidget();
	
	virtual void CreateSpecialWidget();
	virtual void RemoveSpecialWidget();
	
	virtual void CreatePreMatchEnd(const FString& Reason, ETeam WinnerTeam);
	virtual void RemovePreMatchEnd();
	
	virtual void CreateTabMenu();

	virtual void ClientErrorMessage_Implementation(const FString& Message) override;

public:

	ABaseMatchHUD();
	
	UFUNCTION(BlueprintPure)
	UUserWidget* GetMainWidget() const { return CharacterHUD; }
	 
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
	UEndGameWidgetBase* EndGameMatchWidget;
	
	UPROPERTY()
	UEndGameWidgetBase* PreEndMatchWidget;
};
