// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Perception/AIPerceptionTypes.h"
#include "SpaceWar/Controllers/SpecialControllers/Base/SpecialControllerBase.h"
#include "SpaceWar/Pawns/Match/SpecialWeapon/BaseTurelPawn.h"

#include "SpecialTurelController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEWAR_API ASpecialTurelController : public ASpecialControllerBase
{
	GENERATED_BODY()

	ASpecialTurelController();

	void LaunchObjectUsed();
	void UseObject();
	void StopRateDelay();
	void StopObjectUse(bool bRefresh);
	void DropTraceForTarget();

	UFUNCTION(NetMulticast, Unreliable)
	void Test(const FHitResult& Hit);

	UFUNCTION()
	void ObjectDead();

	UFUNCTION()
	void OwnerPlaced(ASpecialWeaponObjectBase* SpecialWeapon);

	AActor* GetFirstPerceptionByTeam();

protected:

	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

protected:

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* Perception;

	UPROPERTY()
	ABaseTurelPawn* TurelPawn;

	FTimerHandle TargetCoolDawnHandle;
	FTimerHandle PerceptionRefreshHandle;
};
