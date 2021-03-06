
#include "DataBaseTransfer.h"
#include "SpaceWar/Objects/Web/WebRequest/AuthorizationRequest.h"
#include "SpaceWar/Objects/Web/WebRequest/RegisterWebRequest.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequesGetHUDServerList.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestCheckServerCreation.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestFindLevelInfo.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetGameListByType.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetServerList.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestReceivingWeaponList.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestServerNameVerification.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestUpdateLevel.h"

// Sets default values for this component's properties
UDataBaseTransfer::UDataBaseTransfer()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UDataBaseTransfer::RegisterUser(const FRegisterInfo& Info, const FDelegateRequestRegisterUserCallBack& CallBack)
{
	auto const Request = NewObject<URegisterWebRequest>(GetOwner());
	Request->AddRegisterKey(Info, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/RegisterUser.php");
}

void UDataBaseTransfer::AuthorizationUser(const FUserInfo& Info, const FDelegateRequestRegisterUserCallBack& CallBack)
{
	auto const Request = NewObject<UAuthorizationRequest>(GetOwner());
	Request->AddAuthorizationValue(Info, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/AuthorizationUser.php");
}

void UDataBaseTransfer::ReceivingServerList(const TArray<FString>& MapNames, const FGetServerListDelegate& CallBack)
{
	auto const Request = NewObject<UWebRequestGetServerList>(GetOwner());
	Request->AddServerListKey(MapNames, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/ReceivingServerList.php");
}

void UDataBaseTransfer::ReceivingWeaponList(const FString& Login, const FReceivingWeaponListDelegate& CallBack)
{
	auto const Request = NewObject<UWebRequestReceivingWeaponList>(GetOwner());
	Request->AddReceivingWeaponListKey(Login, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/ReceivingWeaponList.php");
}

void UDataBaseTransfer::ReceivingCreateServerComplete(const FString& ServerName, const FCreateServerCompelete& CallBack)
{
	auto const Request = NewObject<UWebRequestCheckServerCreation>(GetOwner());
	Request->AddCheckingServerKey(ServerName, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/ReceivingCheckingServerCreation.php");
}

void UDataBaseTransfer::ReceivingServerListByType(const FString& Type, const FGetServerListDelegate CallBack)
{
	auto const Request = NewObject<UWebRequestGetGameListByType>(GetOwner());
	Request->AddMapTypeKey(Type, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/ReceivingServerListByMapType.php");
}

void UDataBaseTransfer::ReceivingServerNameVerification(const FString& Name, const FServerNameVerificationCallback& Callback)
{
	auto const Request = NewObject<UWebRequestServerNameVerification>(GetOwner());
	Request->AddVerificationKey(Name, Callback);
	Request->CollectRequest("127.0.0.1/SpaceWar/ServerNameVerification.php");
}

void UDataBaseTransfer::ReceivingFindHudServerList(const FGetServerHudListDelegate& Callback)
{
	auto const Request = NewObject<UWebRequesGetHUDServerList>(GetOwner());
	Request->AddReceivingWeaponListKey(Callback);
	Request->CollectRequest("127.0.0.1/SpaceWar/GetHUDServers.php");
}

void UDataBaseTransfer::UpdateExpInfo(int32 const CurrentExp, const FString& PlayerName)
{
	auto const Request = NewObject<UWebRequestUpdateLevel>(GetOwner());
	Request->AddUpdateLevelKey(CurrentExp, PlayerName);
	Request->CollectRequest("127.0.0.1/SpaceWar/UpdateLevelInfo.php");
}

void UDataBaseTransfer::GetLevelInfo(const FString& Login, const FFindPlayerLevel& Callback)
{
	auto const Request = NewObject<UWebRequestFindLevelInfo>(GetOwner());
	Request->AddFindLevelInfoKeys(Login, Callback);
	Request->CollectRequest("127.0.0.1/SpaceWar/GetLevelInfo.php");
}
