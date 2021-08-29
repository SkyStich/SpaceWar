// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MapInfo.generated.h"

UENUM(BlueprintType)
enum class EGameType : uint8
{
	None,
	CapturePoint UMETA(DisplayName = "Capture and hold"),
	CaptureFlag UMETA(DisplayName = "Capture the flag")
};

USTRUCT(BlueprintType)
struct FMapData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MapName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MapReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGameType GameType;
};