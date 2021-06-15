// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagForCapture.h"

// Sets default values
AFlagForCapture::AFlagForCapture()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	NetUpdateFrequency = 1.f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionResponseToChannels(ECR_Ignore);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(SphereCollision);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMesh->SetCollisionProfileName("NoCollision", false);
}

// Called when the game starts or when spawned
void AFlagForCapture::BeginPlay()
{
	Super::BeginPlay();
	//GetWorld()->GetCurrentLevel()->GetLevelBlueprints()
}

