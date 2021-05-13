// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseThrowProjectile.generated.h"

UCLASS()
class SPACEWAR_API ABaseThrowProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseThrowProjectile();

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SpecialWeapon")
    void Detonation(float BaseDamage, float const MaxDamageRadius, AController* InstigatorController);
    
public:
    
    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* SkeletalMesh;
};
