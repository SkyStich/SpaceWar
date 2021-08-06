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
		CallBack.Execute(false, "Passwords don't match");
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
	CallBackRequestRegisterUser.OnRequestRegisterUserCallBack.ExecuteIfBound(bResult, ErrorMessage);
}

