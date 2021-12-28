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

	Explosion();
	Destroy();
	ForceNetUpdate();
}
	