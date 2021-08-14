// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateSessionComplete, bool, bResult);

UCLASS()
class SPACEWAR_API USessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	USessionSubsystem();

	void CreateSession(int32 NumPublicConnections, const FString& LevelName, bool LAN);

protected:

	UFUNCTION()
	void OnCreateSessionCompleteEvent(FName SessionName, bool bResult);

public:

	UPROPERTY()
	FCreateSessionComplete OnCreateSessionComplete;

private:

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
};
