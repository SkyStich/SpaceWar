// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableWeaponManager.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
// Sets default values for this component's properties
UEquipableWeaponManager::UEquipableWeaponManager()
{
	CurrentWeapon = nullptr;

	SetIsReplicated(true);
}


// Called when the game starts
void UEquipableWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	if(!WeaponDataAsset) UE_LOG(LogTemp, Error, TEXT("Weapon Data Asset is nullptr"), *GetName());

	//if(GetOwnerRole() == ROLE_Authority) CharacterOwner = Cast<ASpaceWarCharacter>(GetOwner());
}

bool UEquipableWeaponManager::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool ParentReturn = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if(CurrentWeapon) ParentReturn = Channel->ReplicateSubobject(CurrentWeapon, *Bunch, *RepFlags);
	return ParentReturn;
}

void UEquipableWeaponManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipableWeaponManager, CurrentWeapon);
	DOREPLIFETIME_CONDITION(UEquipableWeaponManager, Weapons, COND_OwnerOnly);
}

UBaseWeaponObject* UEquipableWeaponManager::CreateWeaponByName(const FName& Name, EWeaponType Type)
{
	if(!WeaponDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Data asset in null. --CreateWeaponByName"), *GetFullName());
		return nullptr;
	}
	auto const TempWeapon = WeaponDataAsset->CreateWeaponObject(Name, GetWorld(), GetOwner());
	AddWeaponToStorage(Type, TempWeapon);
	return TempWeapon;
}

void UEquipableWeaponManager::AddWeaponToStorage(EWeaponType Key, UBaseWeaponObject* Value)
{
	Weapons.Add(Key, Value);
}

void UEquipableWeaponManager::SetCurrentWeapon(UBaseWeaponObject* NewWeapon)
{
	CurrentWeapon = NewWeapon;
	OnRep_CurrentWeapon();
}

void UEquipableWeaponManager::OnRep_CurrentWeapon()
{
	OnCurrentWeaponChanged.Broadcast(CurrentWeapon);
}
