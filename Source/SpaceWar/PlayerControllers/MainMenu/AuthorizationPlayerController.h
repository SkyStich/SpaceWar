// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuPlayerControllerBase.h"
#include "AuthorizationPlayerController.generated.h"

UCLASS()
class SPACEWAR_API AAuthorizationPlayerController : public AMainMenuPlayerControllerBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

};
