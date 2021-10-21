// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpaceWar/SpaceWarCharacter.h"

#include "HUDWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UHUDWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
private:

	void UpdateAmmoSlot();

public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void RemoveFromParent() override;
	
protected:

	virtual void NativeConstruct() override;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>AmmoWidgetClass;
	
	UPROPERTY()
	UUserWidget* AmmoWidget;

	UPROPERTY()
	ASpaceWarCharacter* OwnerPlayer;
};
