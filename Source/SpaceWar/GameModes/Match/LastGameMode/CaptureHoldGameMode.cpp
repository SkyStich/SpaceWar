// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureHoldGameMode.h"

ACaptureHoldGameMode::ACaptureHoldGameMode()
{
	PointForWin = 1000;
}

void ACaptureHoldGameMode::CharDead(AController* InstigatorController, AController* LoserController, AActor* DamageCauser)
{
	Super::CharDead(InstigatorController, LoserController, DamageCauser);

	RespawnPlayer(LoserController, 5.f);
}