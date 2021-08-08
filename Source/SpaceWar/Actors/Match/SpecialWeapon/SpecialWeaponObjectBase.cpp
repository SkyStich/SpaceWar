// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialWeaponObjectBase.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
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

	ObjectHealthComponent = CreateDefaultSubobject<USpecialObjectHealthComponent>(TEXT("SpecialObjectHealth"));
	
	bReplicates = true;
	NetUpdateFrequency = 10.f;

	Team = ETeam::NoneTeam;

	bObjectConstruct = false;
}

void ASpecialWeaponObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpecialWeaponObjectBase, bObjectConstruct);
	DOREPLIFETIME(ASpecialWeaponObjectBase, Team);
}

void ASpecialWeaponObjectBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ASpecialWeaponObjectBase::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	if(GetInstigator()->IsLocallyControlled())
	{
		SetActorTickEnabled(true);
	}
	else
	{
		SkeletalMesh->SetVisibility(false);
	}

	if(GetLocalRole() == ROLE_Authority)
	{
		bObjectConstruct = true;
		
		if(!GetInstigator()->IsLocallyControlled())
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASpecialWeaponObjectBase::UpdateLocation, 0.2f, true);
		}
	}
}

void ASpecialWeaponObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bObjectConstruct)
	{
		if(!GetInstigator() || !GetInstigator()->GetController())
		{
			Server_InstigatorIsNull();
			return;
		}
		
		UpdateLocation();
	}
}

void ASpecialWeaponObjectBase::Server_InstigatorIsNull_Implementation()
{
	if(!GetInstigator() || !GetInstigator()->GetController())
	{
		if(Controller)
		{
			auto const TempController = Controller;
			Controller->UnPossess();
			TempController->Destroy();
		}
		Destroy(true);
	}
}

void ASpecialWeaponObjectBase::UpdateLocation()
{
	FVector Location = GetInstigator()->GetActorLocation() + FVector(0.f, 0.f, 40.f);
	FVector TraceEnd = GetInstigatorController()->GetControlRotation().Vector() * 230.f + Location;

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
	SetActorRotation(GetInstigator()->GetActorRotation());
}

void ASpecialWeaponObjectBase::OnRep_ObjectConstruct()
{
	if(!bObjectConstruct)
	{
		SetActorTickEnabled(false);
		SkeletalMesh->SetVisibility(true);
	}
}

bool ASpecialWeaponObjectBase::InteractionObject_Implementation(ASpaceWarCharacter* Player)
{	
	if(bObjectConstruct)
	{
		if(Player != GetInstigator()) return false;
	
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		
		UpdateLocation();
		
		bObjectConstruct = false;
		OnRep_ObjectConstruct();
		
		PlaceSucceeded();
		return false;
	}
	return true;
}

void ASpecialWeaponObjectBase::PlaceSucceeded()
{
	if(OwnerController->PlayerState->GetClass()->ImplementsInterface(UGetPlayerTeamInterface::StaticClass()))
	{
		Team = IGetPlayerTeamInterface::Execute_FindPlayerTeam(OwnerController->PlayerState);
	}
	OnPlaceSucceeded.Broadcast(this);
}