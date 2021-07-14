// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInfo.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerId = 0;

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName = FString("");

	UPROPERTY(BlueprintReadOnly)
	int32 LVL = 1;

	UPROPERTY(BlueprintReadWrite)
	TArray<FName> WeaponNames;
};
