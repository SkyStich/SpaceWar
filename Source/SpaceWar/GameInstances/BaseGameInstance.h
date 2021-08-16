// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpaceWar/Components/EquipableWeaponManager.h"
#include "SpaceWar/Enums/SolderClassCategory.h"
#include "BaseGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentSave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESolderCategory Key;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EWeaponType, FName> Value;
};

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
	void SetPlayerName(const FString& NewPlayerName) { PlayerName = NewPlayerName; }
	
	UFUNCTION(BlueprintPure, Category = "GameInstance")
	bool GetWeapons(TMap<EWeaponType, FName>& ReturnMap);
	
	UFUNCTION(BlueprintPure, Category = "GameInstance")
	FName FindWeaponByType(EWeaponType Type, const TMap<EWeaponType, FName>& Map);

	UFUNCTION(BlueprintPure, Category = "AmmunitionWidget")
	ESolderCategory GetCurrentSolderCategory() const { return CurrentSolderCategory; }
	
	UFUNCTION(BlueprintCallable, Category = "AmmunitionWidget")
    void SetCurrentSolderCategory(ESolderCategory NewCategory) { CurrentSolderCategory = NewCategory; }

	UFUNCTION(BlueprintCallable, Category = "AmmunitionWidget")
	void ReplacementWeapon(EWeaponType Key, const FName& NewId);

	UFUNCTION(BlueprintPure, BlueprintCosmetic)
	FString GetSessionKey() const { return SessionID; }
	
	void SetSessionID(const FString& SessionKey) { SessionID = SessionKey; }

protected:

	virtual void Init() override;
	
public:
    	
    UPROPERTY(BlueprintReadWrite)
    TArray<FEquipmentSave> Equipment;

private:

	UPROPERTY()
	FName CurrentArmor;

	UPROPERTY()
	FString SessionID;
	
	UPROPERTY()
	FString PlayerName;

protected:
	
	UPROPERTY(BlueprintReadWrite)
	ESolderCategory CurrentSolderCategory;
};
