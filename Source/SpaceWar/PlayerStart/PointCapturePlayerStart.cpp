// Fill out your copyright notice in the Description page of Project Settings.


#include "PointCapturePlayerStart.h"
#include "SpaceWar/Actors/Match/TeamPoints/TeamPoints.h"

void APointCapturePlayerStart::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority && Point)
	{
		Point->OnOwnerTeamChanged.AddDynamic(this, &APointCapturePlayerStart::OnOwnerTeamChanged);
	}
}

void APointCapturePlayerStart::OnOwnerTeamChanged(ETeam NewOwnerTeam, EPointNumber Number)
{
	if(Number == PointNumber) SpawnTeam = NewOwnerTeam;
}

