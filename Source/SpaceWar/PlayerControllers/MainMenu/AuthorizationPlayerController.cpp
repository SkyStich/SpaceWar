// Fill out your copyright notice in the Description page of Project Settings.


#include "AuthorizationPlayerController.h"
#include "SpaceWar/Objects/Web/WebRequest/AuthorizationRequest.h"
#include "Kismet/GameplayStatics.h"

void AAuthorizationPlayerController::AuthorizationPlayer(const FString& Log, const FString& Pass)
{
	if(GetLocalRole() != ROLE_Authority) return;
	
	auto const Request = NewObject<UAuthorizationRequest>(this);
	Request->AddAuthorizationValue(Log, Pass);
	Request->CollectRequest("127.0.0.1/Authorization.php");
	Request->OnAuthorizationDelegate.BindUObject(this, &AAuthorizationPlayerController::OnAuthorizationEvent);
}

void AAuthorizationPlayerController::Register(const FString& Log, const FString& Pass, const FString RepeatPass)
{
	
}

void AAuthorizationPlayerController::OnAuthorizationEvent(bool IsSucceeded)
{
	GEngine->AddOnScreenDebugMessage(-1,4.f, FColor::Green, FString::Printf(TEXT("Authorixation: %d"), IsSucceeded));
	
	OnAuthorizationAttempt.Broadcast(IsSucceeded);
	Client_AuthorizationAttempt_Implementation(IsSucceeded);
}

void AAuthorizationPlayerController::Client_AuthorizationAttempt_Implementation(bool bResult)
{
	OnAuthorizationAttempt.Broadcast(bResult);
}
