// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMatchSpectator.h"

ABaseMatchSpectator::ABaseMatchSpectator()
{	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SptringArm"));
	SpringArm->TargetArmLength = 450.f;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bReplicates = true;
	NetUpdateFrequency = 5.f;
}

void ABaseMatchSpectator::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseMatchSpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseMatchSpectator::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseMatchSpectator::LookUpAtRate);
}

void ABaseMatchSpectator::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseMatchSpectator::LookUpAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

