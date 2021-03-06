// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	
	CurrentStaminaValue = MaxStaminaValue = 120.f;
	
	IncreasePerSec = 10;
	DecreasePerSec = 15;

	SuperSprintSpeed = 1900.f;
	bCanToggleSuperStamina = true;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UStaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UStaminaComponent, CurrentStaminaValue, COND_OwnerOnly);
	DOREPLIFETIME(UStaminaComponent, bStaminaUse);
	DOREPLIFETIME(UStaminaComponent, bSuperSprintUse);
}

void UStaminaComponent::StartUseStamina()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaHandle);

	bStaminaUse = true;
	OnRep_StaminaUse();
	
	GetWorld()->GetTimerManager().SetTimer(StaminaHandle, this, &UStaminaComponent::DecreaseStamina, 0.2f, true);
}

void UStaminaComponent::StopUseStamina()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaHandle);
	
	bStaminaUse = false;
	OnRep_StaminaUse();

	GetWorld()->GetTimerManager().SetTimer(StaminaHandle, this, &UStaminaComponent::IncreaseStamina, 0.2f, true, 4.f);
}

void UStaminaComponent::IncreaseStamina()
{
	CurrentStaminaValue += IncreasePerSec * 0.2;

	if(CurrentStaminaValue >= MaxStaminaValue)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaHandle);
		CurrentStaminaValue = MaxStaminaValue;
	}
}

void UStaminaComponent::DecreaseStamina()
{
	CurrentStaminaValue -= IncreasePerSec * 0.2;

	if(CurrentStaminaValue <= 0)
	{
		StopUseStamina();
		OnStaminaEnded.Broadcast();
	}
}

void UStaminaComponent::OnRep_StaminaUse()
{
	OnStaminaUsed.Broadcast(bStaminaUse);
}

void UStaminaComponent::Server_StartUseStamina_Implementation()
{
	if(CurrentStaminaValue > 0)
	{
		StartUseStamina();
	}
}

void UStaminaComponent::Server_StopUseStamina_Implementation()
{
	if(!bStaminaUse) return;

	StopUseStamina();
}

void UStaminaComponent::OnRep_SuperSprint()
{
	OnSuperSprintUsed.Broadcast(bSuperSprintUse);

	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;
	TimerDel.BindLambda([&]() -> void
	{
		bCanToggleSuperStamina = true;
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 1.2f, false);
}

void UStaminaComponent::StartUseSuperSprint()
{
	if(bStaminaUse || bSuperSprintUse) return;

	bCanToggleSuperStamina = false;
	bSuperSprintUse = true;
	OnRep_SuperSprint();
}

void UStaminaComponent::StopUseSuperSprint()
{
	if(!bSuperSprintUse) return;

	bCanToggleSuperStamina = false;
	bSuperSprintUse = false;
	OnRep_SuperSprint();
}

