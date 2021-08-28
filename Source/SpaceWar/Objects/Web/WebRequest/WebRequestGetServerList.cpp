#include "WebRequestGetServerList.h"
#include "Dom/JsonValue.h"

void UWebRequestGetServerList::AddServerListKey(const TArray<FString>& MapNames, const FGetServerListDelegate& CallBack)
{
	ServerNames = MapNames;
	OnServerListDelegate = CallBack;
}

void UWebRequestGetServerList::CollectRequest(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> JsonObject = CreateJsonRequest();
	TArray<TSharedPtr<FJsonValue>> NamesArray;

	for(const auto& ByArray : ServerNames)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Name: %s"), *ByArray));
		TSharedPtr<FJsonObject> Object = CreateJsonRequest();
		Object->SetStringField("Map", ByArray);
		TSharedRef<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(Object));
		NamesArray.Add(JsonValueObject);
	}
	JsonObject->SetArrayField("MapNames", NamesArray);
	CallWebScript(ScriptURL, JsonObject);
}

void UWebRequestGetServerList::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
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
	OnServerListDelegate.Execute(ClientServersInfo);
}

void UWebRequestGetServerList::CallJsonFail()
{
	
}