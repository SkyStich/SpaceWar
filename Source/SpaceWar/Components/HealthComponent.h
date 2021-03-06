// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthComponent;

/** For All */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOwnerDead);

/** For owning client */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmorChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOwnerSufferedDamage, const FVector&, DamageDirection);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION(Client, Unreliable)
	void Client_HealthChanged();

	UFUNCTION(Client, Unreliable)
	void Client_ArmorChanged();
	
	UFUNCTION(Client, Unreliable)
	void GetOwnerDamage(const FVector& DamageVector);

	UFUNCTION()
    void OnRep_OwnerDead();

	void StartRegenerationHealth();
	void StartRegenerationArmor();

	void ArmorRegeneration();
	void HealthRegeneration();

	float ArmorResist(float Damage);

	bool IsOwnerDead();
	
	bool CheckForFriendlyFire(AController* DamageReceiver, AController* InstigatorController); // return true, if damage be apply not friend

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	void Init(float const InitMaxArmor, float const InitArmorRegenerationPerSec);

	UFUNCTION(BlueprintPure, Category = "Health|Getting")
	float GetCurrentArmor() const { return CurrentArmor; }

	UFUNCTION(BlueprintPure, Category = "Health|Getting")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Health|Getting")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Health|Getting")
	float GetMaxArmor() const { return MaxArmor; }
	
	UFUNCTION(BlueprintPure, Category = "Health|Getting")
	bool IsOwnerDeath() const { return bOwnerDead; } 

	/** Call on take damage or pick up regeneration object */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void ChangeCurrentHealth(float const Value);

	/** Call on take damage or pick up regeneration object */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void ChangeCurrentArmor(float const Value);

	void FirstAid(float const Value);

	void SetMaxArmor(const int32 NewMaxArmor);

protected:
	
	UFUNCTION()
    void OnPlayerTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);
	
	UFUNCTION()
    void OnPlayerTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnPlayerTakeAnyDamage(AActor* DamagedActor, float BaseDamage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);

	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth;

	UPROPERTY(Replicated)
	float MaxArmor;
	
	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthRegenerationPerSec;

	UPROPERTY(EditAnywhere, Category = "Health")
	float ArmorRegenerationPerSec;

	UPROPERTY(Replicated)
	float CurrentHealth;

	UPROPERTY(Replicated)
	float CurrentArmor;

	UPROPERTY(ReplicatedUsing = OnRep_OwnerDead)
	bool bOwnerDead;

	FTimerHandle RegenerationHandle;
	FTimerHandle PreRegenerationHandle;

public:

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Gelegates")
	FOwnerDead OnOwnerDead;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Gelegates")
	FHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Gelegates")
	FArmorChanged OnArmorChanged;

	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Gelegates")
	FOwnerSufferedDamage OnOwnerSufferedDamage;
};
