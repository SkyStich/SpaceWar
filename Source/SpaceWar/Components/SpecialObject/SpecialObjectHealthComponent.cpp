// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialObjectHealthComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USpecialObjectHealthComponent::USpecialObjectHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CurrentHealth = MaxHealth = 140.f;
	SetIsReplicated(true);
}


// Called when the game starts
void USpecialObjectHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwnerRole() == ROLE_Authority)
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &USpecialObjectHealthComponent::OnTakeDamage);
	}
}

void USpecialObjectHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USpecialObjectHealthComponent, CurrentHealth);
	DOREPLIFETIME(USpecialObjectHealthComponent, bIsDead);
}

void USpecialObjectHealthComponent::OnTakeDamage(AActor* DamageActor, float BaseDamage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	DecreaseHealth(BaseDamage);
}

void USpecialObjectHealthComponent::DecreaseHealth(float Value)
{
	CurrentHealth -= Value;
	if(CurrentHealth <= 0)
	{
		bIsDead = true;
		CurrentHealth = 0;
		OnRep_IsDead();
	}
}

void USpecialObjectHealthComponent::IncreaseHealth(float Value)
{
	CurrentHealth += Value;
	if(CurrentHealth >= MaxHealth)
	{
		CurrentHealth = MaxHealth;
		bIsDead = false;
		OnRep_IsDead();
	}
}

void USpecialObjectHealthComponent::OnRep_IsDead()
{
	if(bIsDead)
	{
		OnHealthEnded.Broadcast();
	}
	else
	{
		OnObjectRefresh.Broadcast();
	}
}



