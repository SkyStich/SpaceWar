// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "SpaceWar/Enums/PlayerTeamEnum.h"
#include "MatchPlayerStartBase.generated.h"

UCLASS()
class SPACEWAR_API AMatchPlayerStartBase : public APlayerStart
{
	GENERATED_BODY()

public:
	


	UFUNCTION()
	bool CheckOnFreePoints() const;


protected:

	virtual void BeginPlay() override;

private:
	
	UPROPERTY()
	bool bPointFree;
};
