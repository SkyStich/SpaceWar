// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuPlayerControllerBase.h"
#include "AuthorizationPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuthorizationAttempt, bool, bResult);

UCLASS()
class SPACEWAR_API AAuthorizationPlayerController : public AMainMenuPlayerControllerBase
{
	GENERATED_BODY()
	
	void OnAuthorizationEvent(bool IsSucceeded);

	UFUNCTION(Client, Reliable)
	void Client_AuthorizationAttempt(bool bResult);

public:

	UFUNCTION(BlueprintCallable, Category = "Authorization")
    void AuthorizationPlayer(const FString& Log, const FString& Pass);

	UFUNCTION(BlueprintCallable, Category = "Authorization")
    void Register(const FString& Log, const FString& Pass, const FString RepeatPass);

public:

	UPROPERTY(BlueprintAssignable)
	FAuthorizationAttempt OnAuthorizationAttempt;
};
