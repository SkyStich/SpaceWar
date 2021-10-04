// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ErrorMessageWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMessageClose);

UCLASS()
class SPACEWAR_API UErrorMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void Init(const FString& Message);

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FMessageClose OnMessageClose;
};
