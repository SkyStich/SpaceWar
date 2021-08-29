// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuPlayerControllerBase.h"
#include "AuthorizationPlayerController.generated.h"

UCLASS(Blueprintable)
class SPACEWAR_API AAuthorizationPlayerController : public AMainMenuPlayerControllerBase
{
	GENERATED_BODY()

public:

	AAuthorizationPlayerController();

	virtual void BeginPlay() override;

};
