// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceWar/PlayerControllers/Match/Base/MatchPlayerControllerBase.h"
#include "SpaceWar/Structs/CreateServerCallBack.h"
#include "SpaceWar/Structs/GetServerListCallBack.h"
#include "SpaceWar/Structs/ReceivingWeaponListCallBack.h"
#include "SpaceWar/Structs/UserInfo.h"
#include "SpaceWar/Structs/RegisterUsersCallBack.h"
#include "SpaceWar/Structs/ServerInfo.h"
#include "ClientServerTransfer.generated.h"

/** moves data between the client and the dedicated server */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEWAR_API UClientServerTransfer : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION(Server, Reliable)
	void Server_SendRegisterInfo(const FRegisterInfo& RegisterInfo);

	UFUNCTION(Server, Reliable)
	void Server_SendAuthorizationInfo(const FUserInfo& Data);

	UFUNCTION(Server, Reliable)
	void Server_SendReceivingServerList(const TArray<FString>& MapNames);

	UFUNCTION(Server, Reliable)
	void Server_SendReceivingWeaponList(const FString& Login);

	UFUNCTION(Server, Reliable)
	void Server_SendReceivingCreateServerComplete(const FString& ServerName);

	UFUNCTION(Server, Reliable)
	void Server_SendReceivingServerListByType(const FString& Type);
	
	UFUNCTION(Server, Reliable)
	void Server_SendReceivingServerNameVerification(const FString& Name);

	UFUNCTION(Server, Reliable)
	void Server_SendReceivingServerHudList();

	UFUNCTION(Server, Reliable)
	void Server_SendReceivingLevelInfo(const FString& Login);

	UFUNCTION()
	void ResponseRegisterUserFromDataBase(bool bResult, const FString& SessionKey, const FString& ErrorMessage);

	UFUNCTION()
	void OnResponseAuthorizationUser(bool bResult, const FString& SessionKey, const FString& ErrorMessage);

	UFUNCTION()
	void OnResponseReceivingServerList(const TArray<FClientServerInfo>& ClientServersInfo);

	UFUNCTION()
	void OnResponseReceivingWeaponList(const TArray<FString>& WeaponList);

	UFUNCTION()
	void OnResponseCreateServerResult(bool bResult, const FString& Address);

	UFUNCTION()
	void OnResponseServerNameVerification(bool bResult);

	UFUNCTION()
	void OnResponseFindServerHudList(TArray<FGetServerHUDListCallBack> HudList);

	UFUNCTION()
	void OnResponseLevelInfo(int32 Level, int32 Exp);
	
	UFUNCTION(Client, Reliable)
	void Client_ResponseRegisterUser(bool bResult, const FString& SessionKey, const FString& ErrorMessage);

	UFUNCTION(Client, Reliable)
    void Client_ResponseAuthorizationUser(bool bResult, const FString& SessionKey, const FString& ErrorMessage);

	UFUNCTION(Client, Reliable)
	void Client_ResponseReceivingServerList(const TArray<FClientServerInfo>& ClientServersInfo);

	UFUNCTION(Client, Reliable)
	void Client_ResponseReceivingWeaponList(const TArray<FString>& WeaponList);

	UFUNCTION(Client, Reliable)
	void Client_CreateServerCompleteResult(bool bResult, const FString& Address);

	UFUNCTION(Client, Reliable)
	void Client_ResponseServerNameVerification(bool bResult);

	UFUNCTION(Client, Reliable)
	void Client_ResponseServerHudList(const TArray<FGetServerHUDListCallBack>& HudList);

	UFUNCTION(Client, Reliable)
	void Client_ResponseLevelInfo(int32 Level, int32 Exp);
public:	

	UClientServerTransfer();

	/** send request on server */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void RequestRegisterUser(const FRegisterInfo& RegisterInfo, const FDelegateRequestRegisterUserCallBack& CallBack);

	/** send request on server */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void RequestAuthorizationUser(const FUserInfo& Info, const FDelegateRequestRegisterUserCallBack& CallBack);

	/** send request on server */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
    void RequestReceivingServerList(const TArray<FString>& MapNames, const FGetServerListDelegate& CallBack);
    
    UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void RequestReceivingWeaponList(const FReceivingWeaponListDelegate& CallBack);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void RequestReceivingCreateServerResult(const FString& ServerName, const FCreateServerCompelete& CallBack);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void RequestReceivingGetServerListByType(const FString& Type, const FGetServerListDelegate& Callback);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void RequestReceivingServerNameVerification(const FString& Name, const FServerNameVerificationCallback& Callback);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void RequestReceivingFindServerHudList(const FGetServerHudListDelegate& Callback);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic)
	void RequestReceivingGetLevelInfo(const FString& Login, const FFindPlayerLevel& Callback);

protected:

	virtual void BeginPlay() override;

public:

	/** on client */
	FCallBackRequestRegisterUser CallBackRequestRegisterUser;
	FGetServerListCallBack ServerListCallBack;
	FReceivingWeaponListCallBack OnReceivingWeaponListCallBack;
	FCreateServerCompelete OnCreateServerComplete;
	FServerNameVerificationCallback OnServerNameVerificationCallback;
	FGetServerHudListDelegate OnGetServerHudListDelegate;
	FFindPlayerLevel OnGetPlayerLevel;
};
