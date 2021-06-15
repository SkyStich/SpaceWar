// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceWar/Actors/Match/CaptureMode/FlagForCapture.h"
#include "SpawnFlagHelpers.generated.h"

UCLASS()
class SPACEWAR_API ASpawnFlagHelpers : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnFlagHelpers();

	UFUNCTION(BlueprintAuthorityOnly)
	void SpawnFlag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Location", meta = (MakeEditWidget = "true"))
	TArray<FVector> SpawnPoints;

	TSubclassOf<AFlagForCapture> FlagClass;
};
