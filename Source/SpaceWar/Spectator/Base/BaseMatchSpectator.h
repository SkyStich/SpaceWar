// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "BaseMatchSpectator.generated.h"

UCLASS()
class SPACEWAR_API ABaseMatchSpectator : public APawn
{
	GENERATED_BODY()

public:

	ABaseMatchSpectator();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	void LookUpAtRate(float Rate);
	void TurnAtRate(float Rate);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;

	float BaseTurnRate;
	float BaseLookUpRate;
};
