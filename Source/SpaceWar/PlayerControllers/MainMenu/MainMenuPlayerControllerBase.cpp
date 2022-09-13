// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayerControllerBase.h"

AMainMenuPlayerControllerBase::AMainMenuPlayerControllerBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true; 
	NetUpdateFrequency = 1.f;

	DataBaseTransfer = CreateDefaultSubobject<UDataBaseTransfer>(TEXT("DataBaseTransfer"));
	ClientServerTransfer = CreateDefaultSubobject<UClientServerTransfer>(TEXT("ClientServerTransfer"));
}

