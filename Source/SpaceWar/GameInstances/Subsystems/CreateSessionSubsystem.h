// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Online.h"
#include "CreateSessionSubsystem.generated.h"



UCLASS()
class SPACEWAR_API UCreateSessionSubsystem : public USubsystem
{
	GENERATED_BODY()

public:

	UCreateSessionSubsystem();

	/**
	 * Function for create game session
	 *	@param		UserId				the user id who sent the creation request 
	 *	@param		SessionName			Name of the session
	 *	@param		bIsPresent			"Is the Session to create a presence Session"
	 *	@param		MaxPlayers			Max player on this session
	 */
	bool CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsPresence, int32 MaxPlayers);
	
	/**	Function fired when a session create request has completed
	 *	@param		SessionName			the name of the session this callback is for
	 *	@param		bWasSuccessful		true if the async action completed without error, false if there was an error
	 */
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	
	/**
	 * Function fired when a session start request has completed
	 * @param		SessionName			the name of the session this callback is for*
	 * @param		bWasSuccessful		true if the async action completed without error, false if there was an error
	 */
	//void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

private:

	/** Called when session be created/starting */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
};
