// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "SpaceWar/GameModes/Match/Base/MatchGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentArmor = MaxArmor = 120.f;
	CurrentHealth = MaxHealth = 100.f;
	SetIsReplicated(true);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwnerRole() == ROLE_Authority)
	{
		GetOwner()->OnTakePointDamage.AddDynamic(this, &UHealthComponent::OnPlayerTakePointDamage);
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHealthComponent, CurrentHealth, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, CurrentArmor, COND_OwnerOnly);
	DOREPLIFETIME(UHealthComponent, bOwnerDead);
}

void UHealthComponent::OnPlayerTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	
}

void UHealthComponent::OnPlayerTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	float const NewDamage = ArmorResist(Damage);

	ChangeCurrentHealth(NewDamage);

	if(IsOwnerDead())
	{
		Cast<AMatchGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->CharDead(InstigatedBy, GetOwner()->GetInstigatorController(), DamageCauser);
		GetOwner()->SetCanBeDamaged(false);
	}
}

float UHealthComponent::ArmorResist(float Damage)
{
	float const TempDamage = CurrentArmor - Damage;
	if(TempDamage <= 0)
	{
		ChangeCurrentArmor(CurrentArmor * -1);
		return TempDamage;
	}
	ChangeCurrentArmor(Damage * -1);
	return  0.f;
}
bool UHealthComponent::IsOwnerDead()
{
	if(CurrentHealth <= 0)
	{
		bOwnerDead = true;
		OnRep_OwnerDead();
		return true;
	}
	return false;
}

void UHealthComponent::OnRep_OwnerDead()
{
	OnOwnerDead.Broadcast();
}

void UHealthComponent::ChangeCurrentArmor(float const Value)
{
	CurrentArmor += Value;
}

void UHealthComponent::ChangeCurrentHealth(float const Value)
{
	CurrentHealth += Value;
}

void UHealthComponent::Client_ArmorChanged_Implementation()
{
	OnArmorChanged.Broadcast();
}

void UHealthComponent::Client_HealthChanged_Implementation()
{
	OnHealthChanged.Broadcast();
}

