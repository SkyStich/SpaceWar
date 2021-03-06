// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectToServerGameMode.h"
#include "Kismet/GameplayStatics.h"

FString AConnectToServerGameMode::GetAddress()
{
	if(UGameplayStatics::HasOption(OptionsString, "ServerIp"))
	{
		return UGameplayStatics::ParseOption(OptionsString, "ServerIp") + ":7777";
	}
	return "None";
}

void AConnectToServerGameMode::BeginPlay()
{
	Super::BeginPlay();
}
