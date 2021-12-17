// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExperienceComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UExperienceComponent : public UActorComponent
{
	GENERATED_BODY()

	class UDataBaseTransfer* GetDataBaseTransfer();

	void ReceivingUpdateLevel();
	
public:	
	// Sets default values for this component's properties
	UExperienceComponent();

	void UpdateExp(const int32 Value);
	
protected:

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
private:

	UPROPERTY(Replicated)
	int32 CurrentLevel;

	UPROPERTY(Replicated)
	int32 CurrentExp;
};
