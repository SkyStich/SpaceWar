// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "SpaceWar/DataAssets/SpecialObjectDataAsset.h"


#include "SpecialObjectManagerComponent.generated.h"

class ASpecialWeaponObjectBase;

/** Controller component */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API USpecialObjectManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION()
	void FinishSpecialSpawnObject(bool bResult, FStringAssetReference Reference, ASpecialWeaponObjectBase* SpecialActor);

public:	
	// Sets default values for this component's properties
	USpecialObjectManagerComponent();

	void DecreasePoints(int32 const Value);
	void IncreasePoints(int32 const Value);

	UFUNCTION(BlueprintPure, Category = "SpecialObject|Getting") 
	int32 GetCurrentSpecialPoints() const { return CurrentSpecialPoints; }

	bool CreateSpecialObject(const FName& ObjectId, const FTransform& Transform);
	
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(Replicated)
	int32 CurrentSpecialPoints;

	UPROPERTY()
	UDataTable* SpecialDataTable;

	FAsyncSpecialSpawnActor SpecialActorDelegate;
};
