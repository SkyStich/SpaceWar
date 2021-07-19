// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"


UCLASS()
class SPACEWAR_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBaseGameInstance();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	void SetCurrentArmor(const FName& Id);

	UFUNCTION(BlueprintPure, Category = "GameInstance")
	FName GetCurrentArmorId() const { return CurrentArmor; }

private:

	UPROPERTY()
	FName CurrentArmor;
	
};
