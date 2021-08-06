// Fill out your copyright notice in the Description page of Project Settings.

#include "AuthorizationPlayerController.h"
#include "SpaceWar/Objects/Web/WebRequest/AuthorizationRequest.h"
#include "Kismet/GameplayStatics.h"

AAuthorizationPlayerController::AAuthorizationPlayerController()
{
	ClientServerTransfer = CreateDefaultSubobject<UClientServerTransfer>(TEXT("ClientServerTransfer"));
	DataBaseTransfer = CreateDefaultSubobject<UDataBaseTransfer>(TEXT("DataBaseTransfer"));
}

void AAuthorizationPlayerController::BeginPlay()
{
	Super::BeginPlay();
}