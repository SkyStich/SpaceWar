// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireParticleTrace.generated.h"

UCLASS()
class SPACEWAR_API AFireParticleTrace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireParticleTrace();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(const FVector& EndLocation, UParticleSystem* Particle);
	
	UFUNCTION()
    void OnPointCaptureCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);


protected:
	
	UPROPERTY()
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnyWhere)
	UParticleSystemComponent* ParticleSystemComponent;

private:

	float Alpha;
	FVector EndParticleLocation;
	
	UPROPERTY(EditAnywhere)
	float InterpSpeed;
};
