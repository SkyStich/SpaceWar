
#include "DataBaseTransfer.h"

#include "SpaceWar/Objects/Web/WebRequest/AuthorizationRequest.h"
#include "SpaceWar/Objects/Web/WebRequest/RegisterWebRequest.h"

// Sets default values for this component's properties
UDataBaseTransfer::UDataBaseTransfer()
{
	PrimaryComponentTick.bCanEverTick = true;

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
