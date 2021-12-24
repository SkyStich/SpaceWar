// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGrenadeRifleProjectile.h"

// Sets default values
ABaseGrenadeRifleProjectile::ABaseGrenadeRifleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionProfileName("NoCollision");
	
	bReplicates = true;
	NetUpdateFrequency = 5.f;
}

// Called when the game starts or when spawned
void ABaseGrenadeRifleProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(GetInstigator())
	CapsuleComponent->IgnoreActorWhenMoving(GetInstigator(), true);

	if(GetLocalRole() == ROLE_Authority)
	{
		CapsuleComponent->OnComponentHit.AddDynamic(this, &ABaseGrenadeRifleProjectile::OnComponentHit);
	}
}

void ABaseGrenadeRifleProjectile::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

