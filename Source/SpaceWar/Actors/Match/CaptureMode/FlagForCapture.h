// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceWaR/GameStates/Match/CaptureOfFlagGameState.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SpaceWar/Enums/PlayerTeamEnum.h"
#include "SpaceWar/Interfaces/CurrentCharacterObjectInterface.h"
#include "FlagForCapture.generated.h"

UCLASS()
class SPACEWAR_API AFlagForCapture : public AActor, public ICurrentCharacterObjectInterface
{
	GENERATED_BODY()

	UFUNCTION()
	void OwnerPlayerDeath();

	UFUNCTION()
	void OnRep_CharOwner();

	UFUNCTION()
	void RoundEnded(const FString& Reason, ETeam WinnerTeam, EReasonForEndOfRound ReasonEndOfRound);
	
public:	
	// Sets default values for this actor's properties
	AFlagForCapture();

	virtual bool InteractionObject_Implementation(ASpaceWarCharacter* Player) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
    void OnPointCaptureCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY(ReplicatedUsing = OnRep_CharOwner)
	ASpaceWarCharacter* OwnerCharacter;

	/** true if a member of the capture team picked up the flag or flag is lost. else if Current location == Default spawn location */
	bool bFlagCaptured;

	FVector DefaultSpawnLocation;
};
