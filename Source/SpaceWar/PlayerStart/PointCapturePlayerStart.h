// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatchPlayerStartBase.h"
#include "PointCapturePlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API APointCapturePlayerStart : public AMatchPlayerStartBase
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void OnOwnerTeamChanged(ETeam NewOwnerTeam, EPointNumber Number);
	
public:

	UFUNCTION(BlueprintPure, Category = "PlayerStart")
	ETeam GetSpawnTeam() const { return SpawnTeam; }
		
	UFUNCTION()
    void SetSpawnTeam(ETeam NewTeam) { SpawnTeam = NewTeam; }

	UFUNCTION(BlueprintPure)
	EPointNumber GetPointNumber() const { return PointNumber; }

protected:

	virtual void BeginPlay() override;

private:
 
	UPROPERTY(EditAnywhere)
	EPointNumber PointNumber;

	UPROPERTY(EditAnywhere)
	ETeam SpawnTeam = ETeam::NoneTeam;

	UPROPERTY(EditAnywhere)
	class ATeamPoints* Point;
};
