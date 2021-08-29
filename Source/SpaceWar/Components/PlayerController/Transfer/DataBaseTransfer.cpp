
#include "DataBaseTransfer.h"
#include "SpaceWar/Objects/Web/WebRequest/AuthorizationRequest.h"
#include "SpaceWar/Objects/Web/WebRequest/RegisterWebRequest.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestCheckServerCreation.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestGetServerList.h"
#include "SpaceWar/Objects/Web/WebRequest/WebRequestReceivingWeaponList.h"

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
