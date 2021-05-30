// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTurelPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ABaseTurelPawn::ABaseTurelPawn()
{
	bObjectUsed = false;

	BaseDamage = 5.f;
	DelayBeforeUse = 0.1f;
}

void ABaseTurelPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseTurelPawn, bObjectUsed);
	DOREPLIFETIME(ABaseTurelPawn, TargetActor);
}


void ABaseTurelPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseTurelPawn::OnRep_ObjectUsed()
{
	if(GetNetMode() == NM_DedicatedServer) return;
	
	if(bObjectUsed)
	{
		GetWorld()->GetTimerManager().SetTimer(PlayVisualHandle, this, &ABaseTurelPawn::PlayVisualEffects, DelayBeforeUse, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(PlayVisualHandle);
	}
}

void ABaseTurelPawn::PlaceSucceeded()
{
	Super::PlaceSucceeded();
}
