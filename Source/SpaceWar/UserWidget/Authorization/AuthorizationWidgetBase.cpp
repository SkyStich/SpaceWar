// Fill out your copyright notice in the Description page of Project Settings.


#include "AuthorizationWidgetBase.h"

bool UAuthorizationWidgetBase::CheckingForValidSymbol(const FText& Text)
{
	if(Text.IsEmpty()) return false;

	for(auto ByArray : Text.ToString())
	{
		if(IgnoredSymbol.GetCharArray().Contains(ByArray)) return false;
	}
	return true;
}
