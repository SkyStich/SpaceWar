// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaUsed, bool, bStaminaState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSuperSprintUsed, bool, bSprintState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaEnded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

	void IncreaseStamina();
	void DecreaseStamina();

	void StartUseStamina();

	UFUNCTION()
	void OnRep_StaminaUse();

	UFUNCTION()
	void OnRep_SuperSprint();

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	void StopUseStamina();
	void StartUseSuperSprint();
	void StopUseSuperSprint();

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_StartUseStamina();

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void Server_StopUseStamina();
	
	UFUNCTION(BlueprintPure)
	float GetCurrentStaminaValue() const { return CurrentStaminaValue; }
	
	UFUNCTION(BlueprintPure)
	bool IsStaminaUse() const { return bStaminaUse; }
	
	bool IsSuperSprintUsed() const { return bSuperSprintUse; }
	bool IsCanToggleSuperStamina() const { return bCanToggleSuperStamina; }
	float GetSuperSprintSpeed() const { return SuperSprintSpeed; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UPROPERTY(BlueprintAssignable, Category = "Stamina|Delegate")
	FStaminaUsed OnStaminaUsed;

	UPROPERTY(BlueprintAssignable, Category = "Stamina|Delegate")
	FStaminaEnded OnStaminaEnded;
	
	UPROPERTY(BlueprintAssignable, Category = "Sprint|Delegate")
	FSuperSprintUsed OnSuperSprintUsed;

private:

	UPROPERTY(EditAnywhere, Category = "StaminaComponent|Editable")
	float MaxStaminaValue;

	UPROPERTY(EditAnywhere, Category = "StaminaComponent|Editable")
	float IncreasePerSec;
	
	UPROPERTY(EditAnywhere, Category = "StaminaComponent|Editable")
	float DecreasePerSec;

	UPROPERTY(EditAnywhere, Category = "StaminaComponent|Speed")
	float SuperSprintSpeed;

	UPROPERTY(ReplicatedUsing = OnRep_StaminaUse)
	bool bStaminaUse;
	
	UPROPERTY(ReplicatedUsing = OnRep_SuperSprint)
	bool bSuperSprintUse;

	UPROPERTY(Replicated)
	float CurrentStaminaValue;

	bool bCanToggleSuperStamina;

	FTimerHandle StaminaHandle;
};
