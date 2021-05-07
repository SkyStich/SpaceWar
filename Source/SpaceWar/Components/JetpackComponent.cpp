// Fill out your copyright notice in the Description page of Project Settings.


#include "JetpackComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UJetpackComponent::UJetpackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);
	bJetpackInCoolDawn = false;
	Straight = 2200.f;
	ZVelocity = 0.65;
}

void UJetpackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UJetpackComponent, bJetpackInCoolDawn, COND_OwnerOnly);
}

bool UJetpackComponent::StartUseJetpack(const FVector& ForwardVector, FVector& LaunchVector)
{
	if(bJetpackInCoolDawn) return false;

	FVector const Vector = ForwardVector * Straight;
	
	LaunchVector = Vector;
	LaunchVector.Z = ZVelocity * Straight;

	bJetpackInCoolDawn = true;
	
	FTimerHandle RefreshHandle;
	GetWorld()->GetTimerManager().SetTimer(RefreshHandle, this, &UJetpackComponent::ReloadJetpack, 8.f, false);
	return true;
}

void UJetpackComponent::ReloadJetpack()
{
	bJetpackInCoolDawn = false;
}