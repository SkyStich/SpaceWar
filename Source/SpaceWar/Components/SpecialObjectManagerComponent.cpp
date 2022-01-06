// Fill out your copyright notice in the Description page of Project Settings.

#include "SpecialObjectManagerComponent.h"
#include "DrawDebugHelpers.h"
#include "SpaceWar/Actors/Match/SpecialWeapon/SpecialWeaponObjectBase.h"
#include "Camera/CameraComponent.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceWar/DataAssets/SpecialObjectDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"

USpecialObjectManagerComponent::USpecialObjectManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	static ConstructorHelpers::FObjectFinder<UDataTable>SpecialTable(TEXT("/Game/ThirdPersonCPP/DataTables/DT_SpecialWeaponObject"));
	if(SpecialTable.Succeeded())
	{
		SpecialDataTable = SpecialTable.Object;
	}

	CurrentSpecialPoints = 50000;
}

// Called when the game starts
void USpecialObjectManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	SpecialActorDelegate.BindUFunction(this, TEXT("FinishSpecialSpawnObject"));
}

void USpecialObjectManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(USpecialObjectManagerComponent, CurrentSpecialPoints, COND_OwnerOnly);
}

void USpecialObjectManagerComponent::DecreasePoints(int32 const Value)
{
	CurrentSpecialPoints -= Value;
}

void USpecialObjectManagerComponent::IncreasePoints(int32 const Value)
{
	CurrentSpecialPoints += Value;
}

bool USpecialObjectManagerComponent::CreateSpecialObject(const FName& ObjectId, const FTransform& Transform, AMatchPlayerControllerBase* OwnerController)
{
	static const FString ContextString("DT_SpecialWeaponObject");
	auto const SpecialRow = SpecialDataTable->FindRow<FSpecialObject>(ObjectId, *ContextString);

	if(CurrentSpecialPoints >= SpecialRow->Price)
	{
		if(USpecialObjectDataAsset::AsyncSpawnActor(GetWorld(), SpecialRow->SpecialActor, Transform, OwnerController, SpecialActorDelegate))
		{
			DecreasePoints(SpecialRow->Price);
		}
		return true;
	}
	
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("do not have CurrentSpecialPoints for buy object --USpecialObjectManagerComponent::CreateSpecialObject"));
	return false;
}

void USpecialObjectManagerComponent::FinishSpecialSpawnObject(bool bResult, FStringAssetReference Reference, ASpecialWeaponObjectBase* SpecialActor)
{

}