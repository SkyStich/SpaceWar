// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGrenadeRifleProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseGrenadeRifleProjectile::ABaseGrenadeRifleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCollisionProfileName("BlockAll");
	RootComponent = CapsuleComponent;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionProfileName("NoCollision");
	
	bReplicates = true;
	NetUpdateFrequency = 1.f;
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

void ABaseGrenadeRifleProjectile::Explosion(float const DamageFallOf)
{
	FVector const Origin = GetActorLocation();
	AController* DamageInstigator = Cast<AController>(GetOwner());
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamageWithFalloff(this, BaseDamage, BaseDamage * 0.35, Origin, Radius * 0.35, Radius, DamageFallOf, UDamageType::StaticClass(), IgnoredActors, this, DamageInstigator, ECC_WorldStatic);
}

void ABaseGrenadeRifleProjectile::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

