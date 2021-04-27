// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

/** For All */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOwnerDead);

/** For owning client */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmorChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	void ChangeCurrentHealth(float const Value);
	void ChangeCurrentArmor(float const Value);

	float ArmorResist(float Damage);

	UFUNCTION()
	void OnRep_OwnerDead();

	bool IsOwnerDead();

	UFUNCTION(Client, Unreliable)
	void Client_HealthChanged();

	UFUNCTION(Client, Unreliable)
	void Client_ArmorChanged();

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintPure, Category = "Health|Getting")
	float GetCurrentArmor() const { return CurrentArmor; }

	UFUNCTION(BlueprintPure, Category = "Health|Getting")
	float GetCurrentHealth() const { return CurrentHealth; }

protected:
	
	UFUNCTION()
    void OnPlayerTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
	
	UFUNCTION()
    void OnPlayerTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser);

	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxArmor;

	UPROPERTY(Replicated)
	float CurrentHealth;

	UPROPERTY(Replicated)
	float CurrentArmor;

	UPROPERTY(ReplicatedUsing = OnRep_OwnerDead)
	bool bOwnerDead;

public:

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Gelegates")
	FOwnerDead OnOwnerDead;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Gelegates")
	FHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Gelegates")
	FArmorChanged OnArmorChanged;
};
