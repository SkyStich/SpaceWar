// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"
#include "TeamPoints.generated.h"

UCLASS(Abstract)
class SPACEWAR_API ATeamPoints : public AActor
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_TeamPoints();
	
	void LaunchPointCapture(ETeam CaptureTeam);
	void StopPointCapture();
	void RefreshLaunchCapture();

	void IncreaseCurrentValueCapture();
	void DecreaseCurrentValueCapture();

	void StartAddPoint();
	void UpdateTeamPoints(class AOnlineMatchGameModeBase* OnlineGameMode);
	
public:	
	// Sets default values for this actor's properties
	ATeamPoints();
	
	UFUNCTION(BlueprintPure, Category = "Getting")
	int32 GetCurrentValueCapture() const { return CurrentValueCapture; }

	UFUNCTION(BlueprintPure, Category = "Getting")
	ETeam GetOwnerTeam() const { return OwnerTeam; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnPointCaptureCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPointCaptureCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* PointCaptureCollision;
	
	UPROPERTY(ReplicatedUsing = OnRep_TeamPoints)
	ETeam OwnerTeam;

	UPROPERTY(Replicated)
	bool bIsCapture;

	UPROPERTY(Replicated)
	float CurrentValueCapture;

	UPROPERTY()
	TArray<AController*> OwnersController;
	
	int32 CurrentAmountOwnerInPoint;
	int32 CurrentAmountEnemyAtPoint;

	FTimerHandle CaptureHandle;
	FTimerHandle AddPointHandle;
};
