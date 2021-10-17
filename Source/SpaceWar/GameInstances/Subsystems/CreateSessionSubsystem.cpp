// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateSessionSubsystem.h"

UCreateSessionSubsystem::UCreateSessionSubsystem()
{
	/** Bind the Delegate on event function */
//	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UCreateSessionSubsystem::OnCreateSessionComplete);
//	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UCreateSessionSubsystem::OnStartOnlineGameComplete);
}

bool UCreateSessionSubsystem::CreateSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsPresence, int32 MaxPlayers)
{
	/** Get current online subsystem */
	IOnlineSubsystem* const OnlineSubsystem = IOnlineSubsystem::Get();

	if(OnlineSubsystem)
	{
		// Get the Session Interface
		IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
		if(SessionPtr.IsValid() && UserId.IsValid())
		{
			/** Set session settings */
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = false;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
			SessionSettings->Set(SETTING_MAPNAME, FString("NewMap"), EOnlineDataAdvertisementType::ViaOnlineService);
			OnCreateSessionCompleteDelegateHandle = SessionPtr->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			return SessionPtr->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		UE_LOG(LogOnlineGame, Error, TEXT("Online subsystem is null. Create server complete with fail"));
	}
	return false;
}

void UCreateSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	/** get online subsystem */
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr const SessionPtr = OnlineSubsystem->GetSessionInterface();
		if(SessionPtr.IsValid())
		{
			SessionPtr->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}
	if(bWasSuccessful)
	{
		//UGameplayStatics::OpenLevel(GetWorld(),  )
	}
}