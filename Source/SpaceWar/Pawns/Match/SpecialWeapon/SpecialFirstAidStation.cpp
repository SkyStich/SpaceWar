// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialFirstAidStation.h"
#include "SpaceWar/Interfaces/FirstAidInterface.h"

ASpecialFirstAidStation::ASpecialFirstAidStation()
{
	FirstAidSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	FirstAidSphereComponent->SetupAttachment(SkeletalMesh);
	FirstAidSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	FirstAidSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	RegenerationValue = 15.f;
}

void ASpecialFirstAidStation::BeginPlay()
{
	Super::BeginPlay();

}

void ASpecialFirstAidStation::PlaceSucceeded()
{
	Super::PlaceSucceeded();

	FirstAidSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpecialFirstAidStation::OnOverlapBegin);
	FirstAidSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ASpecialFirstAidStation::OnOverlapEnd);
}

void ASpecialFirstAidStation::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->ImplementsInterface(UFirstAidInterface::StaticClass()))
	{
		IFirstAidInterface::Execute_StartPlayerFirstAid(OtherActor, Team, RegenerationValue);
	}
}

void ASpecialFirstAidStation::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->GetClass()->ImplementsInterface(UFirstAidInterface::StaticClass()))
	{
		IFirstAidInterface::Execute_StopPlayerFirstAid(OtherActor);
	}
}


