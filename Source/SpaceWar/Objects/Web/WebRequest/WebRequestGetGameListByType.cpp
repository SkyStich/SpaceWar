// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestGetGameListByType.h"

void UWebRequestGetGameListByType::AddMapTypeKey(const FString& MapType, const FGetServerListDelegate& OnServerInfo)
{
	GameType = MapType;
	Callback = OnServerInfo;
}

void UWebRequestGetGameListByType::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	JsonObject->SetStringField("GameType", GameType);
	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestGetGameListByType::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	TArray<FString>Names;
	TArray<FString> Addresses;
	TArray<FString> MapNames;
	JsonResponse->TryGetStringArrayField("ServerNames", Names);
	JsonResponse->TryGetStringArrayField("MapNames", MapNames);
	JsonResponse->TryGetStringArrayField("ServerAddresses", Addresses);

	TArray<FClientServerInfo> ClientServersInfo;

	for(int32 i = 0; i < Addresses.Num(); i++)
	{
		ClientServersInfo.Add(FClientServerInfo(Names[i], MapNames[i], Addresses[i]));
	}
	Callback.Execute(ClientServersInfo);
}

void UWebRequestGetGameListByType::CallJsonFail()
{
	
}
