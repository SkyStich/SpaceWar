// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceWar/Structs/GetServerListCallBack.h"
#include "SpaceWar/Structs/ReceivingWeaponListCallBack.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"
#include "SpaceWar/Structs/RegisterUsersCallBack.h"
#include "SpaceWar/Structs/UserInfo.h"
#include "DataBaseTransfer.generated.h"

/** only server */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UDataBaseTransfer : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UDataBaseTransfer();

	UFUNCTION()
	void RegisterUser(const FRegisterInfo& Info, const FDelegateRequestRegisterUserCallBack& CallBack);

	UFUNCTION()
	void AuthorizationUser(const FUserInfo& Info, const FDelegateRequestRegisterUserCallBack& CallBack);

	UFUNCTION()
	void ReceivingServerList(const TArray<FString>& MapNames, const FGetServerListDelegate& CallBack);

	UFUNCTION()
	void ReceivingWeaponList(const FString& Login, const FReceivingWeaponListDelegate& CallBack);

	UFUNCTION()
	void ReceivingCreateServerComplete(const FString& ServerName, const FCreateServerCompelete& CallBack);
};
