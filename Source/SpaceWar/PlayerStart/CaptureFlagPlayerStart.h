// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatchPlayerStartBase.h"
#include "CaptureFlagPlayerStart.generated.h"

UCLASS()
class SPACEWAR_API ACaptureFlagPlayerStart : public AMatchPlayerStartBase
{
	GENERATED_BODY()
	
public:
	
	bool GetIsSecurityTeam() const { return IsSecurityTeam; }

private:

	UPROPERTY(EditAnywhere)
	bool IsSecurityTeam;
};
