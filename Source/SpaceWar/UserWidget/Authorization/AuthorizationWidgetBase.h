// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuthorizationWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SPACEWAR_API UAuthorizationWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:

	/** check on valid symbol in editable */
	UFUNCTION(BlueprintCallable, Category = "UserWidget")
	bool CheckingForValidSymbol(const FText& Text);

protected:

	/** add symbol without space (/'"\!) */
	UPROPERTY(EditAnywhere, Category = "UserWidget")
	FString IgnoredSymbol;
};
