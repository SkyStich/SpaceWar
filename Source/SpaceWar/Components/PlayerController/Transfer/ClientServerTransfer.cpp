// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientServerTransfer.h"
#include "DataBaseTransfer.h"
#include "Net/UnrealNetwork.h"

UClientServerTransfer::UClientServerTransfer()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UClientServerTransfer::BeginPlay()
{
	Super::BeginPlay();
}

void UClientServerTransfer::Server_SendRegisterInfo_Implementation(const FRegisterInfo& RegisterInfo)
{
	if(RegisterInfo.Password != RegisterInfo.RepeatPass)
	{
		CallBackRequestRegisterUser.OnRequestRegisterUserCallBack.Execute(false, "Passwords don't match");
		return;
	}
	
	FDelegateRequestRegisterUserCallBack RequestRegisterUserCallBack;
	RequestRegisterUserCallBack.BindUFunction(this, "ResponseRegisterUserFromDataBase");

	UDataBaseTransfer* DataBaseTransfer = GetOwner()->FindComponentByClass<UDataBaseTransfer>();
	if(DataBaseTransfer)
	{
		DataBaseTransfer->RegisterUser(RegisterInfo, RequestRegisterUserCallBack);
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("UClientServerTransfer::Server_SendRegisterInfo_Implementation --Player controller not have component UDataBaseTransfer"))
}

void UClientServerTransfer::ResponseRegisterUserFromDataBase(bool bResult, const FString& ErrorMessage)
{
	Client_ResponseRegisterUser(bResult, ErrorMessage);
}

void UClientServerTransfer::RequestRegisterUser(const FRegisterInfo& RegisterInfo, const FDelegateRequestRegisterUserCallBack& CallBack)
{
	if(RegisterInfo.Password != RegisterInfo.RepeatPass)
	{
		CallBack.ExecuteIfBound(false, "Passwords don't match");
		return;
	}

	CallBackRequestRegisterUser.OnRequestRegisterUserCallBack = CallBack;

	/** Hashing password */
	FRegisterInfo Data = RegisterInfo;
	Data.Password = FMD5::HashAnsiString(*RegisterInfo.Password);
	Data.RepeatPass = FMD5::HashAnsiString(*Data.RepeatPass);
	
	Server_SendRegisterInfo(Data);
}

void UClientServerTransfer::Client_ResponseRegisterUser_Implementation(bool bResult, const FString& ErrorMessage)
{
	CallBackRequestRegisterUser.OnRequestRegisterUserCallBack.Execute(bResult, ErrorMessage);
}

void UClientServerTransfer::Client_ResponseAuthorizationUser_Implementation(bool bResult, const FString& ErrorMessage)
{
	CallBackRequestRegisterUser.OnRequestRegisterUserCallBack.ExecuteIfBound(bResult, ErrorMessage);
}

void UClientServerTransfer::RequestAuthorizationUser(const FUserInfo& Info, const FDelegateRequestRegisterUserCallBack& CallBack)
{
	CallBackRequestRegisterUser.OnRequestRegisterUserCallBack = CallBack;
	FUserInfo Data;
	Data.Login = Info.Login;
	Data.Password = FMD5::HashAnsiString(*Info.Password);
	Server_SendAuthorizationInfo(Data);
}

void UClientServerTransfer::OnResponseAuthorizationUser(bool bResult, const FString& ErrorMessage)
{
	Client_ResponseAuthorizationUser(bResult, ErrorMessage);
}

void UClientServerTransfer::Server_SendAuthorizationInfo_Implementation(const FUserInfo& Data)
{
	FDelegateRequestRegisterUserCallBack AuthorizationCallBack;
	AuthorizationCallBack.BindUFunction(this, "OnResponseAuthorizationUser");

	UDataBaseTransfer* Transfer = GetOwner()->FindComponentByClass<UDataBaseTransfer>();
	if(Transfer)
	{
		Transfer->AuthorizationUser(Data, AuthorizationCallBack);
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("UClientServerTransfer::Server_SendRegisterInfo_Implementation --Player controller not have component UDataBaseTransfer"))
}

void UClientServerTransfer::RequestReceivingServerList(const FGetServerListDelegate& CallBack)
{
	ServerListCallBack.OnGetServerListDelegate = CallBack;

	Server_SendReceivingServerList();
}

void UClientServerTransfer::Server_SendReceivingServerList_Implementation()
{
	FGetServerListDelegate CallBack;
	CallBack.BindUFunction(this, "OnResponseReceivingServerList");

	UDataBaseTransfer* DataBaseTransfer = GetOwner()->FindComponentByClass<UDataBaseTransfer>();
	if(DataBaseTransfer)
	{
		DataBaseTransfer->ReceivingServerList(CallBack);
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("UClientServerTransfer::Server_SendRegisterInfo_Implementation --Player controller not have component UDataBaseTransfer"));
}

void UClientServerTransfer::OnResponseReceivingServerList(const TArray<FClientServerInfo>& ClientServersInfo)
{
	Client_ResponseReceivingServerList(ClientServersInfo);
}

void UClientServerTransfer::Client_ResponseReceivingServerList_Implementation(const TArray<FClientServerInfo>& ClientServersInfo)
{
	bool bResult = ServerListCallBack.OnGetServerListDelegate.ExecuteIfBound(ClientServersInfo);
}

