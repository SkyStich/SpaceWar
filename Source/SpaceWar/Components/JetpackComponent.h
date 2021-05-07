// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JetpackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UJetpackComponent : public UActorComponent
{
	GENERATED_BODY()
	
	void ReloadJetpack();

public:	

	UJetpackComponent();
	
	bool IsAbleToUseJetpack() const { return !bJetpackInCoolDawn; }
	bool StartUseJetpack(const FVector& ForwardVector, FVector& LaunchVector);

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(Replicated)
	bool bJetpackInCoolDawn;

	UPROPERTY(EditAnywhere)
	float Straight;

	UPROPERTY(EditAnywhere)
	float ZVelocity;
};
