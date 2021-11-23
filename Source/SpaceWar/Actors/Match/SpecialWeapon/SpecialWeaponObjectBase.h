// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "SpaceWar/Interfaces/CurrentCharacterObjectInterface.h"
#include "SpaceWar/Interfaces/GetDamageCauserInfo.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"
#include "SpaceWar/Components/SpecialObject/SpecialObjectHealthComponent.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"

#include "SpecialWeaponObjectBase.generated.h"

class ASpaceWarCharacter;
class ASpecialWeaponObjectBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlaceSucceeded, ASpecialWeaponObjectBase*, SpecialActor);

UCLASS()
class SPACEWAR_API ASpecialWeaponObjectBase : public APawn, public ICurrentCharacterObjectInterface
{
	GENERATED_BODY()
	
	UFUNCTION()
	void UpdateLocation();

	UFUNCTION()
	void OnHealthEnded();
	
	UFUNCTION(Server, Unreliable)
	void Server_InstigatorIsNull();
	
public:	
	// Sets default values for this actor's properties
	ASpecialWeaponObjectBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool InteractionObject_Implementation(ASpaceWarCharacter* Player) override;
	virtual void Destroyed() override;
	void SetOwnerController(AMatchPlayerControllerBase* NewOwnerController) { OwnerController = NewOwnerController; }
	void UpdateLocation(const FVector& Location) { SetActorLocation(Location); }

	UFUNCTION(BlueprintPure)
	ETeam GetTeam() const { return Team; }
 
	USpecialObjectHealthComponent* GetObjectHealthComponent() const { return ObjectHealthComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintAuthorityOnly)
	virtual void PlaceSucceeded();
	
	UFUNCTION()
	virtual void OnRep_ObjectConstruct();
	
public:

	FPlaceSucceeded OnPlaceSucceeded;

protected:

	UPROPERTY()
	AMatchPlayerControllerBase* OwnerController;

	UPROPERTY(Replicated)
	ETeam Team;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpecialObjectHealthComponent* ObjectHealthComponent;
	
	UPROPERTY(ReplicatedUsing = OnRep_ObjectConstruct)
	bool bObjectConstruct;
	
	UPROPERTY(BlueprintReadOnly)
	USoundCue* DestroySound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* DestroyParticle;
};
