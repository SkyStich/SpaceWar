// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMatchHUD.h"
#include "CaptureAndHoldHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ACaptureAndHoldHUD : public ABaseMatchHUD
{
	GENERATED_BODY()

	UFUNCTION()
	void PreparationSpawnCharacter();

	UFUNCTION()
	void PreMatchEnd(const FString& Reason, ETeam WinnerTeam);

	void ShowPreparationWidget();
	void HiddenPreparationWidget();

protected:

	virtual void CreateSpectatorWidgets() override;
	virtual void BeginPlay() override;
	virtual void NewOwningPlayerPawn(APawn* Pawn) override;
	virtual void CreatePreparationWidget() override;
	virtual void OnPreparationStartGameEvent(bool bResult) override;

public:

	ACaptureAndHoldHUD();
};
