// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceWar/Structs/UserInfo.h"
#include "SpaceWar/Structs/RegisterUsersCallBack.h"
#include "ClientServerTransfer.generated.h"

/** moves data between the client and the dedicated server */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UClientServerTransfer : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION(Server, Reliable)
	void Server_SendRegisterInfo(const FRegisterInfo& RegisterInfo);

	UFUNCTION()
	void ResponseRegisterUserFromDataBase(bool bResult, const FString& ErrorMessage);

public:	

	UClientServerTransfer();

	/** send request on server */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void RequestRegisterUser(const FRegisterInfo& RegisterInfo, const FDelegateRequestRegisterUserCallBack& CallBack);
	
	UFUNCTION(Client, Reliable)
	void Client_ResponseRegisterUser(bool bResult, const FString& ErrorMessage);

protected:

	virtual void BeginPlay() override;

public:

	FCallBackRequestRegisterUser CallBackRequestRegisterUser;
};
