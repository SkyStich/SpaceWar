// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"

#include "SpecialObjectHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectRefresh);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API USpecialObjectHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	void DecreaseHealth(float Value);
	void IncreaseHealth(float Value);

	UFUNCTION()
	void OnRep_IsDead();

public:	
	// Sets default values for this component's properties
	USpecialObjectHealthComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const { return MaxHealth; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTakeDamage(AActor* DamageActor, float BaseDamage, const UDamageType* DamageType,  AController* InstigatorController, AActor* DamageCauser);

public:

	UPROPERTY(BlueprintAssignable)
	FHealthEnded OnHealthEnded;
	
	UPROPERTY(BlueprintAssignable)
	FObjectRefresh OnObjectRefresh;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth;

	UPROPERTY(Replicated)
	float CurrentHealth;

	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
	bool bIsDead;
};
