// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/ScrollBar.h"
#include "Components/ScrollBox.h"



#include "MatchChatWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API UMatchChatWidgetBase : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION()
	void HiddenMessage(UWidget* MessageWidget, UBorder* Border);

protected:

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void AutoHiddenMessage(UWidget* MessageWidget, UBorder* Border);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void ShowAllMessage(UScrollBox* ScrollBox);
	
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void HiddenAllMessage(UScrollBox* ScrollBox);

public:

	UFUNCTION(BlueprintNativeEvent)
	void ShowChat(bool bOnlyTeam);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HiddenChat();

	bool IsHidden() const { return bHidden; }

private:

	bool bHidden;
};
