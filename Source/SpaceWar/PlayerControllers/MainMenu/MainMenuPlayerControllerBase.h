// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceWar/Structs/UserInfo.h"
#include "MainMenuPlayerControllerBase.generated.h"

UCLASS()
class SPACEWAR_API AMainMenuPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	AMainMenuPlayerControllerBase();
};
