// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "BaseMatchSpectator.generated.h"

UCLASS()
class SPACEWAR_API ABaseMatchSpectator : public ASpectatorPawn
{
	GENERATED_BODY()

public:

	ABaseMatchSpectator();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;
};
