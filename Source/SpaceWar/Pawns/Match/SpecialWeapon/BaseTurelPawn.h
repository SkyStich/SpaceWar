// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceWar/Actors/Match/SpecialWeapon/SpecialWeaponObjectBase.h"
#include "BaseTurelPawn.generated.h"

class ASpecialTurelController;

UCLASS()
class SPACEWAR_API ABaseTurelPawn : public ASpecialWeaponObjectBase
{
	GENERATED_BODY()
	
	UFUNCTION()
    void OnRep_ObjectUsed();

public:

	ABaseTurelPawn();
	
	UFUNCTION(BlueprintPure)
	AActor* GetTargetActor() const { return TargetActor; }

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void PlayVisualEffects();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PlaceSucceeded() override;
	
protected:

	UPROPERTY(EditAnywhere, Category = "Firing")
	float DelayBeforeUse;
	
	UPROPERTY(EditAnywhere, Category = "Firing")
	float BaseDamage;
	
	UPROPERTY(ReplicatedUsing = OnRep_ObjectUsed)
	bool bObjectUsed;
	
	UPROPERTY(Replicated)
    AActor* TargetActor;
    
	FTimerHandle PlayVisualHandle;

	friend ASpecialTurelController;
};
