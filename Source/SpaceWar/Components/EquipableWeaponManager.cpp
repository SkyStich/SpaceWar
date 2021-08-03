// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableWeaponManager.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/Objects/WeaponsThrow/ThrowWeaponBase.h"

// Sets default values for this component's properties
UEquipableWeaponManager::UEquipableWeaponManager()
{
	CurrentWeapon = nullptr;
	bWeaponSelect = false;
	SetIsReplicated(true);
}

// Called when the game starts
void UEquipableWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	if(!WeaponDataAsset) UE_LOG(LogTemp, Error, TEXT("Weapon Data Asset is nullptr"), *GetName());
}

bool UEquipableWeaponManager::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool ParentReturn = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	TArray<UObject*> ReplicationArray;
	if(CurrentWeapon) ReplicationArray.Add(CurrentWeapon);
	if(ThrowWeaponBase) ReplicationArray.Add(ThrowWeaponBase);
	for(const auto& ByArray : Weapons) if(ByArray.Value) ReplicationArray.Add(ByArray.Value);

	ParentReturn = Channel->ReplicateSubobjectList(ReplicationArray, *Bunch, *RepFlags);
	return ParentReturn;
}

void UEquipableWeaponManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipableWeaponManager, CurrentWeapon);
	DOREPLIFETIME(UEquipableWeaponManager, bWeaponSelect);
	DOREPLIFETIME(UEquipableWeaponManager, ThrowWeaponBase);
	DOREPLIFETIME_CONDITION(UEquipableWeaponManager, Weapons, COND_OwnerOnly);
}

URangeWeaponObjectBase* UEquipableWeaponManager::CreateWeaponByName(const FName& Name, EWeaponType Type)
{
	if(!WeaponDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Data asset in null. --CreateWeaponByName"), *GetFullName());
		return nullptr;
	}
	auto const TempWeapon = WeaponDataAsset->CreateWeaponObject(Name, GetWorld(), GetOwner());
	if(!TempWeapon) return nullptr;
	
	AddWeaponToStorage(Type, TempWeapon);
	return TempWeapon;
}

UBaseWeaponObject* UEquipableWeaponManager::FindFromWeapon(EWeaponType Type)
{
	auto const Finder = Weapons.FindByPredicate([&](FWeapons Param) -> bool { return Type == Param.Key; });
	return Finder ? Finder->Value : nullptr;
}

void UEquipableWeaponManager::AddToWeapons(EWeaponType Type, UBaseWeaponObject* Value)
{
	Weapons.Add(FWeapons(Type, Value));
}

void UEquipableWeaponManager::RemoveFromWeapons(EWeaponType Type)
{
	Weapons.RemoveAll([&](FWeapons& Weapon) -> bool { return Weapon.Key == Type; });
}

void UEquipableWeaponManager::AddWeaponToStorage(EWeaponType Key, UBaseWeaponObject* Value)
{
	AddToWeapons(Key, Value);
}

void UEquipableWeaponManager::SetCurrentWeapon(UBaseWeaponObject* NewWeapon)
{
	if(!NewWeapon) return;
	
	CurrentWeapon = NewWeapon;
	OnRep_CurrentWeapon();
}

void UEquipableWeaponManager::OnRep_CurrentWeapon()
{
	OnCurrentWeaponChanged.Broadcast(CurrentWeapon);
}

void UEquipableWeaponManager::OnRep_WeaponSelect()
{
	OnWeaponSelect.Broadcast(bWeaponSelect);
}
 
void UEquipableWeaponManager::SelectWeapon(EWeaponType NewType)
{
	auto const NewWeapon = FindFromWeapon(NewType);
	if(!NewWeapon) return;

	if(NewWeapon != CurrentWeapon)
	{
		bWeaponSelect = true;
		OnRep_WeaponSelect();
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UEquipableWeaponManager::CurrentWeaponUnEquip, NewWeapon);
		GetWorld()->GetTimerManager().SetTimer(SelectWeaponHandle, TimerDel, CurrentWeapon->GetSelectTime(), false);
	}
}

void UEquipableWeaponManager::CurrentWeaponUnEquip(UBaseWeaponObject* NewWeapon)
{
	GetWorld()->GetTimerManager().ClearTimer(SelectWeaponHandle);
	SetCurrentWeapon(NewWeapon);
	GetWorld()->GetTimerManager().SetTimer(SelectWeaponHandle, this, &UEquipableWeaponManager::FinishWeaponSelect, NewWeapon->GetSelectTime(), false);
}

void UEquipableWeaponManager::FinishWeaponSelect()
{
	bWeaponSelect = false;
	OnRep_WeaponSelect();
}

void UEquipableWeaponManager::Server_SelectWeapon_Implementation(EWeaponType NewType)
{
	if(!bWeaponSelect)
	{
		SelectWeapon(NewType);
	}
}

void UEquipableWeaponManager::OwnerWeaponSelect(EWeaponType NewType)
{
	if(!bWeaponSelect)
	{
		Server_SelectWeapon(NewType);
	}
}

bool UEquipableWeaponManager::CreateThrow(const FName Name)
{
	if(!WeaponDataAsset)
	{
		UE_LOG(LogActorComponent, Error, TEXT("-- DataAsset is null: CreateThrow"), *GetFullName());
		return false;
	}
	ThrowWeaponBase = WeaponDataAsset->CreateThrowWeaponObject(Name, GetWorld(), GetOwner());
	return ThrowWeaponBase != nullptr;
}

void UEquipableWeaponManager::OnThrowUsed(bool bUsed)
{
	
}

void UEquipableWeaponManager::OnRep_ThrowWeapon()
{
	if(ThrowWeaponBase != nullptr)
	{
		ThrowWeaponBase->OnWeaponUsed.AddDynamic(this, &UEquipableWeaponManager::OnThrowUsed);
	}
}

void UEquipableWeaponManager::Server_ReplacementWeapon_Implementation(EWeaponType Key, const FName& Id)
{
	WeaponReplacement(Key, Id);
}

void UEquipableWeaponManager::WeaponReplacement(EWeaponType NewType, const FName& Id)
{
	/** current weapon for replacement */
	auto const OldPlayerWeapon = FindFromWeapon(NewType);

	for(const auto& ByArray : Weapons)
	{
		if(ByArray.Value->GetWeaponName() == Id) return;
	}

	auto const TempWeapon = WeaponDataAsset->CreateWeaponObject(Id, GetWorld(), GetOwner());

	if(!TempWeapon || !OldPlayerWeapon) return;

	RemoveFromWeapons(NewType);
	AddToWeapons(NewType, TempWeapon);
	if(OldPlayerWeapon == CurrentWeapon) SetCurrentWeapon(TempWeapon);
	OldPlayerWeapon->ConditionalBeginDestroy();
}
