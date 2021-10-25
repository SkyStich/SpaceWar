// Fill out your copyright notice in the Description page of Project Settings.


#include "FireParticleTrace.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFireParticleTrace::AFireParticleTrace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	RootComponent = ParticleSystemComponent;

	InterpSpeed = 8.f;
}

// Called every frame
void AFireParticleTrace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float const TempAlpha = FMath::FInterpConstantTo(Alpha, 1.f, DeltaTime, 8.f);
	SetActorLocation(FMath::Lerp(GetActorLocation(), EndParticleLocation, TempAlpha));

	if(GetActorLocation().Equals(EndParticleLocation, 0.2f))
	{
		Destroy();
	}
}

void AFireParticleTrace::Init(const FVector& EndLocation, UParticleSystem* Particle)
{
	EndParticleLocation = EndLocation;
	ParticleSystemComponent->SetTemplate(Particle);
	SetTickableWhenPaused(false);
}

void AFireParticleTrace::OnPointCaptureCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	Destroy();
}
