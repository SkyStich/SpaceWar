// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseGrenadeRifleProjectile.generated.h"

UCLASS()
class SPACEWAR_API ABaseGrenadeRifleProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGrenadeRifleProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	float Radius;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta=(AllowPrivateAccess ="true"))
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta=(AllowPrivateAccess ="true"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta=(AllowPrivateAccess ="true"))
	UCapsuleComponent* CapsuleComponent;
};
