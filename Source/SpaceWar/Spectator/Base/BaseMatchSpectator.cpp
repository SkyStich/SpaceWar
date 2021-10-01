// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMatchSpectator.h"

ABaseMatchSpectator::ABaseMatchSpectator()
{	
	/*SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SptringArm"));
	SpringArm->TargetArmLength = 450.f;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetupAttachment(GetMeshComponent());*/

	bReplicates = true;
	NetUpdateFrequency = 5.f;
}

void ABaseMatchSpectator::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseMatchSpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(!PlayerInputComponent)
	{
		PlayerInputComponent = NewObject<UInputComponent>(UInputComponent::StaticClass());
		PlayerInputComponent->RegisterComponent();
	}
	check(PlayerInputComponent);
}

