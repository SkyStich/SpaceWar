// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnFlagHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "SpaceWar/GameStates/Match/CaptureOfFlagGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASpawnFlagHelpers::ASpawnFlagHelpers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NetUpdateFrequency = 1.f;

	ConstructorHelpers::FClassFinder<AFlagForCapture>FlagClassFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/Actors/CaptureMode/BP_FlagForCapture"));
	if(FlagClassFinder.Succeeded()) FlagClass = FlagClassFinder.Class;
}

// Called when the game starts or when spawned
void ASpawnFlagHelpers::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		auto const GS = Cast<ACaptureOfFlagGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if(!GS) return;

		GS->OnRoundPreparation.AddDynamic(this, &ASpawnFlagHelpers::SpawnFlag);
	}
}

void ASpawnFlagHelpers::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetActorLocation(FVector::ZeroVector);
}

void ASpawnFlagHelpers::SpawnFlag()
{
	int32 const Index = UKismetMathLibrary::RandomIntegerInRange(0, SpawnPoints.Num() - 1);
	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = this;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto const Flag = GetWorld()->SpawnActor<AFlagForCapture>(FlagClass, SpawnPoints[Index], FRotator::ZeroRotator, SpawnParam);
	if(!Flag)
	{
		UE_LOG(LogActor, Error, TEXT("-- Flag for capture is not spawned! ASpawnFlagHelpers::SpawnFlag"), *GetName());	
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("SpawnFlag"));
}


