#include "WebRequestGetServerList.h"

void UWebRequestGetServerList::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();

	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestGetServerList::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	TArray<FString>Names;
	TArray<FString> Addresses;
	JsonResponse->TryGetStringArrayField("ServerNames", Names);
	JsonResponse->TryGetStringArrayField("ServerAddresses", Addresses);

	TArray<FClientServerInfo> ClientServersInfo;

	for(int32 i = 0; i < Addresses.Num(); i++)
	{
		ClientServersInfo.Add(FClientServerInfo(Names[i], Addresses[i]));
	}
	OnServerListDelegate.Execute(ClientServersInfo);
}

void UWebRequestGetServerList::CallJsonFail()
{
	
}