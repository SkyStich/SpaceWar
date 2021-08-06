
#include "DataBaseTransfer.h"
#include "SpaceWar/Objects/Web/WebRequest/RegisterWebRequest.h"

// Sets default values for this component's properties
UDataBaseTransfer::UDataBaseTransfer()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDataBaseTransfer::RegisterUser(const FRegisterInfo& Info, const FDelegateRequestRegisterUserCallBack& CallBack)
{
	//todo Check the correctness of the incoming data
	
	auto const Request = NewObject<URegisterWebRequest>(GetOwner());
	Request->AddRegisterKey(Info, CallBack);
	Request->CollectRequest("127.0.0.1/SpaceWar/RegisterUser.php");
}
