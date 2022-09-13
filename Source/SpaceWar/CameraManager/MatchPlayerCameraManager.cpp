// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchPlayerCameraManager.h"
#include "Camera/CameraModifier.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SpaceWar/SpaceWarCharacter.h"
#include "SpaceWar/Components/HealthComponent.h"

AMatchPlayerCameraManager::AMatchPlayerCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	
	static ConstructorHelpers::FObjectFinder<UMaterial>MaterialFinder(TEXT("/Game/ThirdPersonCPP/PostProcess/M_PostProcessDamageNotify"));
	if(MaterialFinder.Succeeded()) DamageMaterial = MaterialFinder.Object;

	static ConstructorHelpers::FClassFinder<UMatineeCameraShake>DamageCameraShakeFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/Camera/CameraShake/BP_DamageCameraShake"));
	if(DamageCameraShakeFinder.Succeeded()) DamageCameraShakeClass = DamageCameraShakeFinder.Class;

	DamageLerp = 0.f;
}

void AMatchPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
	if(!GetOwningPlayerController()) return;
	GetOwningPlayerController()->GetOnNewPawnNotifier().AddUObject(this, &AMatchPlayerCameraManager::OnNewPawn);
}

void AMatchPlayerCameraManager::OnNewPawn(APawn* NewPawn)
{
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Hello"), true, true, FColor::Red);
	if(!NewPawn) return;
	auto const Player = Cast<ASpaceWarCharacter>(NewPawn);
	if(Player && !Player->IsPendingKill())
	{
		DamageDynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), DamageMaterial);
		Player->GetFollowCamera()->AddOrUpdateBlendable(DamageDynamicInstance);
		Player->GetAimCamera()->AddOrUpdateBlendable(DamageDynamicInstance);
		Player->GetHealthComponent()->OnOwnerSufferedDamage.BindUFunction(this, "OnOwnerSufferedDamage");
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void AMatchPlayerCameraManager::OnOwnerSufferedDamage(const FVector& DamageDirection)
{
	DamageLerp = 1.f;

	/** Calculate damage direction for material */
	FQuat const First = FQuat(GetOwningPlayerController()->GetPawn()->GetActorRotation());
	FQuat const Second = FQuat(FRotator(0.f, 90.f, 0.f));
	auto const Direction = FRotator(First * Second).UnrotateVector(DamageDirection);
	DamageDynamicInstance->SetVectorParameterValue("Direction", FLinearColor(Direction.X, Direction.Y, 0.f, 0.f));

	/** start camera shake on take damage */
	StartMatineeCameraShake(DamageCameraShakeClass);
	
	/** On tick */
	SetActorTickEnabled(true);
}

void AMatchPlayerCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!DamageDynamicInstance) return;

	/** Set param on damage material */
	DamageLerp = FMath::FInterpConstantTo(DamageLerp, 0, DeltaSeconds, 1.f);
	DamageLerp = FMath::Clamp(DamageLerp, 0.f, 1.f);
	DamageDynamicInstance->SetScalarParameterValue("Lerp", DamageLerp);

	if(DamageLerp <= 0)
	{
		SetActorTickEnabled(false);
	}
}
