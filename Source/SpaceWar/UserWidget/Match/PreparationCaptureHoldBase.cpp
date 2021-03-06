// Fill out your copyright notice in the Description page of Project Settings.


#include "PreparationCaptureHoldBase.h"
#include "SpaceWar/Interfaces/PreparationWidgetInterface.h"
#include "SpaceWar/PlayerControllers/Match/Last/CaptureHoldController.h"

bool UPreparationCaptureHoldBase::SpawnPlayer()
{
	auto const PC = Cast<ACaptureHoldController>(GetOwningPlayer());
	return PC->SpawnPlayerByPoint(IPreparationWidgetInterface::Execute_FindSpawnPointNumber(this));
}
