// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"
#include "TeamPoints.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOwnerTeamChanged, ETeam, NewOwnerTeam, EPointNumber, PointNumber);

UCLASS(Abstract)
class SPACEWAR_API ATeamPoints : public AActor
{
	GENERATED_BODY()

	UFUNCTION()
	void OnRep_TeamPoints();

	UFUNCTION()
	void MatchEnd(const FString& Reason, ETeam WinnerTeam);
	
	void LaunchPointCapture(ETeam CaptureTeam);
	void StopPointCapture();
	void RefreshLaunchCapture();

	void IncreaseCurrentValueCapture();
	void DecreaseCurrentValueCapture();

	void StartAddPoint();
	void UpdateTeamPoints(class AOnlinetMatchGameStateBase* OnlineGameState);
	void UpdateSpecialPoints();
	
public:	
	// Sets default values for this actor's properties
	ATeamPoints();
	
	UFUNCTION(BlueprintPure, Category = "Getting")
	int32 GetCurrentValueCapture() const { return CurrentValueCapture; }

	UFUNCTION(BlueprintPure, Category = "Getting")
	ETeam GetOwnerTeam() const { return OwnerTeam; }
	
	UFUNCTION(BlueprintPure, Category = "Getting")
	EPointNumber GetPointNumber() const { return PointNumber; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnPointCaptureCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPointCaptureCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:

	UPROPERTY(BlueprintAssignable)
	FOwnerTeamChanged OnOwnerTeamChanged;
	
private:

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* PointCaptureCollision;
	
	UPROPERTY(ReplicatedUsing = OnRep_TeamPoints)
	ETeam OwnerTeam;

	UPROPERTY(Replicated)
	bool bIsCapture;

	UPROPERTY(Replicated)
	int32 CurrentValueCapture;

	UPROPERTY(EditAnywhere)
	EPointNumber PointNumber;

	UPROPERTY()
	TArray<AController*> OwnersController;
	
	int32 CurrentAmountOwnerInPoint;
	int32 CurrentAmountEnemyAtPoint;

	FTimerHandle CaptureHandle;
	FTimerHandle AddPointHandle;
	FTimerHandle AddPointsForOwnerHandle;
};
