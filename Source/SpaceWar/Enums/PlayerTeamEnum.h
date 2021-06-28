// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerTeamEnum.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	NoneTeam,
    TeamA,
    TeamB
};

UENUM(BlueprintType)
enum class EPointNumber : uint8
{
	PointA,
	PointB,
	PointC
};