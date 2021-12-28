// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeStandartProjectile.h"

AGrenadeStandartProjectile::AGrenadeStandartProjectile()
{
	TimeBeforeExplosion = 5.f;
}

void AGrenadeStandartProjectile::BeginPlay()
{
	Super::BeginPlay();

	auto ExplosionDel = [&]() -> void
	{
		Explosion();
		Destroy();
		ForceNetUpdate();
	};

	if(GetLocalRole() == ROLE_Authority)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindLambda(ExplosionDel);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, TimeBeforeExplosion, false);
	}
}

void AGrenadeStandartProjectile::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnComponentHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);	
}