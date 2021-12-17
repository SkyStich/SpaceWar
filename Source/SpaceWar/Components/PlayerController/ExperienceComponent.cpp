// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceComponent.h"
#include "GameFramework/PlayerState.h"
#include "Transfer/DataBaseTransfer.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UExperienceComponent::UExperienceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

UDataBaseTransfer* UExperienceComponent::GetDataBaseTransfer()
{
	UDataBaseTransfer* Transfer = nullptr;
	if(GetOwner())
	{
		Transfer = GetOwner()->FindComponentByClass<UDataBaseTransfer>();
	}
	return Transfer;
}

void UExperienceComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UExperienceComponent, CurrentLevel, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UExperienceComponent, CurrentExp, COND_OwnerOnly);
}

void UExperienceComponent::UpdateExp(const int32 Value)
{
	CurrentExp += Value;
	if(CurrentExp > (2200 * CurrentLevel + 500))
	{
		//Update level
		CurrentLevel++;
	}
	ReceivingUpdateLevel();
}

void UExperienceComponent::ReceivingUpdateLevel()
{
	auto const Transfer = GetDataBaseTransfer();
	if(Transfer)
	{
		auto const PC = Cast<APlayerController>(GetOwner());
		if(PC) Transfer->UpdateExpInfo(CurrentExp, PC->PlayerState->GetPlayerName());
	}
	UE_LOG(LogActorComponent, Error, TEXT("DataBaseTransfer in null -- UExperienceComponent::ReceivingUpdateLevel()"));
}
