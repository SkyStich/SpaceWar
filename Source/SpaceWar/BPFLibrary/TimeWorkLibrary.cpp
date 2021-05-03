// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeWorkLibrary.h"

void UTimeWorkLibrary::ConvertSecondOnTimeStandard(int32 Seconds, FString& ReturnValue)
{
	/** Get minutes and seconds from total seconds */
	int32 const Minutes = Seconds / 60;
	int32 const Sec = Minutes > 0 ? Seconds % (Minutes * 60) : Seconds;

	FString StringMin;
	FString StringSec;
	
	Minutes < 10 ? StringMin = "0" + FString::FromInt(Minutes) : StringMin = FString::FromInt(Minutes);
	Seconds < 10 ? StringSec = "0" + FString::FromInt(Seconds) : StringSec = FString::FromInt(Seconds);

	ReturnValue = StringMin + ":" + StringSec;
}
