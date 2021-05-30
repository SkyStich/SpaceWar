// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialControllerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "SpaceWar/PlayerStates/Match/Base/OnlinePlayerStateBase.h"

ASpecialControllerBase::ASpecialControllerBase()
{
	
}

void ASpecialControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}



