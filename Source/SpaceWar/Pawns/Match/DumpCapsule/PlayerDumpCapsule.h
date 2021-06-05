// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerDumpCapsule.generated.h"

UCLASS()
class SPACEWAR_API APlayerDumpCapsule : public APawn
{
	GENERATED_BODY()

public:

	APlayerDumpCapsule();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	
	virtual void BeginPlay() override;
};
