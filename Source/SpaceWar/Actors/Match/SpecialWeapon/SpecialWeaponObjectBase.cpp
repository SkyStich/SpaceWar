// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialWeaponObjectBase.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "SpaceWar/SpaceWarCharacter.h"

// Sets default values
ASpecialWeaponObjectBase::ASpecialWeaponObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickInterval(0.05f);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	
	bReplicates = true;
	NetUpdateFrequency = 10.f;

	bObjectConstruct = false;
}

void ASpecialWeaponObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpecialWeaponObjectBase, bObjectConstruct);
}

void ASpecialWeaponObjectBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ASpecialWeaponObjectBase::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	if(GetLocalRole() == ROLE_Authority)
	{
		OwnerController = GetInstigator()->GetController();
		bObjectConstruct = true;
	}
	
	if(GetInstigator()->GetController())
	{
		SetActorTickEnabled(true);
	}
}

void ASpecialWeaponObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bObjectConstruct)
	{
		if(!GetInstigator())
		{
			Destroy(true);
			return;
		}
		
		if(GetInstigator()->IsLocallyControlled())
		{
			UpdateLocation();
		}
	}
}

void ASpecialWeaponObjectBase::UpdateLocation()
{
	FVector Location = GetInstigator()->GetActorLocation() + FVector(0.f, 0.f, 40.f);
	FVector TraceEnd = GetInstigator()->GetController()->GetControlRotation().Vector() * 230.f + Location;

	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetInstigator());
	Params.AddIgnoredActor(this);

	FHitResult NewHit;

	if(GetWorld()->LineTraceSingleByChannel(OutHit, Location, TraceEnd, ECC_Visibility, Params))
	{

		GetWorld()->LineTraceSingleByChannel(NewHit, OutHit.Location + FVector(0.f, 0.f, 500.f), OutHit.Location - FVector(0.f, 0.f, 50.f), ECC_Visibility, Params);
	}
	else
	{
		GetWorld()->LineTraceSingleByChannel(NewHit, OutHit.TraceEnd, OutHit.TraceEnd - FVector(0.f, 0.f, 500.f), ECC_Visibility, Params);
	}
	
	DrawDebugLine(GetWorld(), Location, TraceEnd, FColor::Green, false, 0.5f);
	DrawDebugLine(GetWorld(), NewHit.TraceStart, NewHit.TraceEnd, FColor::Blue, false, 0.5f);
	DrawDebugSphere(GetWorld(), NewHit.ImpactPoint, 8.f, 8, FColor::Yellow);
	
	SetActorLocation(NewHit.ImpactPoint);
}

bool ASpecialWeaponObjectBase::InteractionObject_Implementation(ASpaceWarCharacter* Player)
{
	if(bObjectConstruct)
	{
		SetActorTickEnabled(false);
		if(GetLocalRole() == ROLE_Authority)
		{
			bObjectConstruct = false;
			OnPlaceSucceeded.Broadcast(this);
		}
	}
	return true;
}
