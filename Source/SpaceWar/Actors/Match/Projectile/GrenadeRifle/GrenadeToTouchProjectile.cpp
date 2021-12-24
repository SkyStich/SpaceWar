// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeToTouchProjectile.h"
#include "Kismet/GameplayStatics.h"

void AGrenadeToTouchProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AGrenadeToTouchProjectile::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnComponentHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	FVector const Origin = GetActorLocation();
	AController* DamageInstigator = Cast<AController>(GetOwner());
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamageWithFalloff(this, BaseDamage, BaseDamage * 0.35, Origin, Radius * 0.35, Radius, 2.5f, UDamageType::StaticClass(), IgnoredActors, this, DamageInstigator, ECC_WorldStatic);
	Destroy();
	
	ForceNetUpdate();
}
	