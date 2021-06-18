// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "SpaceWar/GameModes/Match/Base/MatchGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "SpaceWar/Interfaces/GetPlayerTeamInterface.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentArmor = MaxArmor = 220.f;
	CurrentHealth = MaxHealth = 160.f;
	HealthRegenerationPerSec = 1.f;
	ArmorRegenerationPerSec = 2.f;
	
	SetIsReplicated(true);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwnerRole() == ROLE_Authority)
	{
		GetOwner()->OnTakePointDamage.AddDynamic(this, &UHealthComponent::OnPlayerTakePointDamage);
		GetOwner()->OnTakeRadialDamage.AddDynamic(this, &UHealthComponent::OnPlayerTakeRadialDamage);
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnPlayerTakeAnyDamage);
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHealthComponent, CurrentHealth, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, CurrentArmor, COND_OwnerOnly);
	DOREPLIFETIME(UHealthComponent, bOwnerDead);
}

void UHealthComponent::FirstAid(float const Value)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("FIRSTAID!!!!"));
	if(CurrentHealth >= MaxHealth)
	{
		ChangeCurrentArmor(Value);
		return;
	}
	
	int32 const TempValue = CurrentHealth + Value - MaxHealth;
	ChangeCurrentHealth(Value);
	if(TempValue > 0)
	{
		ChangeCurrentArmor(TempValue);
	}
}

void UHealthComponent::OnPlayerTakeAnyDamage(AActor* DamagedActor, float BaseDamage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	if(CheckForFriendlyFire(DamagedActor->GetInstigatorController(), InstigatorController)) return;
	
	GetWorld()->GetTimerManager().ClearTimer(PreRegenerationHandle);
	GetWorld()->GetTimerManager().ClearTimer(RegenerationHandle);

	auto PreStart = [&]() -> void
	{
		CurrentHealth < MaxHealth ? StartRegenerationHealth() : StartRegenerationArmor();
	};
	
	FTimerDelegate TimerDel;
	TimerDel.BindLambda(PreStart);
	GetWorld()->GetTimerManager().SetTimer(PreRegenerationHandle, TimerDel, 4.f, false);
}

void UHealthComponent::OnPlayerTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	if(CheckForFriendlyFire(DamagedActor->GetInstigatorController(), InstigatedBy)) return;
	
	GetWorld()->GetTimerManager().ClearTimer(RegenerationHandle);
	
	float const NewDamage = ArmorResist(Damage);

	ChangeCurrentHealth(NewDamage);

	if(IsOwnerDead())
	{
		Cast<AMatchGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->CharDead(InstigatedBy, GetOwner()->GetInstigatorController(), DamageCauser);
		GetOwner()->SetCanBeDamaged(false);
	}
}

void UHealthComponent::OnPlayerTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	//if(CheckForFriendlyFire(DamagedActor->GetInstigatorController(), InstigatedBy)) return;
	
	if(BoneName == "b_head")
	{
		Damage *= 2.f;
	}
	GetWorld()->GetTimerManager().ClearTimer(RegenerationHandle);
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
	CurrentArmor = FMath::Clamp(CurrentArmor, 0.f, MaxArmor);
}

void UHealthComponent::ChangeCurrentHealth(float const Value)
{	
	CurrentHealth += Value;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
}

void UHealthComponent::Client_ArmorChanged_Implementation()
{
	OnArmorChanged.Broadcast();
}

void UHealthComponent::Client_HealthChanged_Implementation()
{
	OnHealthChanged.Broadcast();
}

void UHealthComponent::StartRegenerationHealth()
{
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &UHealthComponent::HealthRegeneration);
	GetWorld()->GetTimerManager().SetTimer(RegenerationHandle, TimerDel, 0.05f, true);
}

void UHealthComponent::StartRegenerationArmor()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &UHealthComponent::ArmorRegeneration);
	GetWorld()->GetTimerManager().SetTimer(RegenerationHandle, TimerDelegate, 0.05f, true);
}

void UHealthComponent::HealthRegeneration()
{
	CurrentHealth += HealthRegenerationPerSec;
	if(CurrentHealth >= MaxHealth)
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenerationHandle);
		StartRegenerationArmor();
	}
}

void UHealthComponent::ArmorRegeneration()
{
	CurrentArmor += ArmorRegenerationPerSec;
	if(CurrentArmor >= MaxArmor)
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenerationHandle);
	}
}

bool UHealthComponent::CheckForFriendlyFire(AController* DamageReceiver, AController* InstigatorController)
{
	if(!InstigatorController->PlayerState->GetClass()->ImplementsInterface(UGetPlayerTeamInterface::StaticClass())) return true;

	if(IGetPlayerTeamInterface::Execute_FindPlayerTeam(DamageReceiver->PlayerState) == IGetPlayerTeamInterface::Execute_FindPlayerTeam(InstigatorController->PlayerState)) return false;

	return true;
}
