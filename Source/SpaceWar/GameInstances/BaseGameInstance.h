// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpaceWar/Components/EquipableWeaponManager.h"
#include "SpaceWar/Enums/SolderClassCategory.h"
#include "BaseGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FCurrentWeaponKey
{
	GENERATED_BODY()
	
	FCurrentWeaponKey() : Key(EWeaponType::Unknown), Value("") {}
	FCurrentWeaponKey(EWeaponType NewType, const FName& NewName) : Key(NewType), Value(NewName) {}

	EWeaponType Key;
	FName Value;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCurrentExpChanged, int32, NewExp, int32, ValueToAdd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentLevelChanged, int32, NewLevel);

USTRUCT(BlueprintType)
struct FEquipmentSave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESolderCategory Key;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCurrentWeaponKey> Value;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFirstWeaponChanged, const FName&, Id);

UCLASS()
class SPACEWAR_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBaseGameInstance();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	void SetCurrentArmor(const FName& Id);

	UFUNCTION(BlueprintPure, Category = "GameInstance")
	FName GetCurrentArmorId() const { return CurrentArmor; }
	
	UFUNCTION(BlueprintPure, Category = "GameInstance")
	FString GetPlayerName() const { return PlayerName; }

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	void SetPlayerName(const FString& NewPlayerName);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "GameInstance")
	void LoadPlayerData();

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "GameInstance")
	void SaveFullPlayerData();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	void SetCurrentMainHUBServerAddress(const FString& NewAddress) { CurrentMainHUBServerAddress = NewAddress; }
	
	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	void SetThrowWeapon(const FName& NewId) { CurrentThrowWeapon = NewId; }

	UFUNCTION(BlueprintPure)
	TMap<EWeaponType, FName> GetWeapons() const { return Weapons; }
	
	UFUNCTION(BlueprintPure, Category = "GameInstance")
	FName FindWeaponByType(EWeaponType Type, const TMap<EWeaponType, FName>& Map);

	UFUNCTION(BlueprintPure, Category = "AmmunitionWidget")
	ESolderCategory GetCurrentSolderCategory() const { return CurrentSolderCategory; }

	UFUNCTION(BlueprintPure, Category = "GameInstance")
	FString GetCurrentMainHUBServerAddress() const { return CurrentMainHUBServerAddress; }

	UFUNCTION(BlueprintPure, Category = "GameInstance")
	FName GetCurrentThrowWeapon() const { return CurrentThrowWeapon; }
	
	UFUNCTION(BlueprintCallable, Category = "AmmunitionWidget")
    void SetCurrentSolderCategory(ESolderCategory NewCategory) { CurrentSolderCategory = NewCategory; }

	UFUNCTION(BlueprintCallable, Category = "AmmunitionWidget")
	void ReplacementWeapon(EWeaponType Key, const FName& NewId);

	UFUNCTION(BlueprintPure, BlueprintCosmetic)
	FString GetSessionKey() const { return SessionID; }

	UFUNCTION(BlueprintCallable)
	void SetExp(int32 Value);

	UFUNCTION(BlueprintCallable)
	void SetLevel(int32 Value);

	UFUNCTION(BlueprintPure)
	int32 GetExp() const { return CurrentExp; }

	UFUNCTION(BlueprintPure)
	int32 GetLevel() const { return CurrentLevel; }
	
	void SetSessionID(const FString& SessionKey) { SessionID = SessionKey; }
	void SetPlayerId(int32 const Id) { PlayerId = Id; }
	virtual void Shutdown() override;
	virtual void Init() override;

protected:

	virtual void OnStart() override;
	
public:
    	
    UPROPERTY(BlueprintReadWrite)
    TMap<EWeaponType, FName> Weapons;

	UPROPERTY(BlueprintAssignable)
	FFirstWeaponChanged OnFirstWeaponChanged;

	UPROPERTY(BlueprintAssignable)
	FCurrentExpChanged OnCurrentExpChanged;

	UPROPERTY(BlueprintAssignable)
	FCurrentLevelChanged OnCurrentLevelChanged;
	
private:

	UPROPERTY(SaveGame)
	FName CurrentArmor;
	
	UPROPERTY()
	FName CurrentThrowWeapon;

	UPROPERTY()
	FString SessionID;
	
	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	FString CurrentMainHUBServerAddress;
	
	UPROPERTY()
	int32 PlayerId;

	UPROPERTY()
	int32 CurrentLevel;

	UPROPERTY()
	int32 CurrentExp;

protected:
	
	UPROPERTY(BlueprintReadWrite)
	ESolderCategory CurrentSolderCategory;
};
