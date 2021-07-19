// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"

UBaseGameInstance::UBaseGameInstance()
{
	//Teat
	CurrentArmor = "Base";
}

void UBaseGameInstance::SetCurrentArmor(const FName& Id)
{
	CurrentArmor = Id;
}
