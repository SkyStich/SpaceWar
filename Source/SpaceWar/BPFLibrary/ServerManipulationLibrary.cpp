// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerManipulationLibrary.h"
#include "GenericPlatform/GenericPlatformProcess.h"

void UServerManipulationLibrary::LaunchServer(const FString& MapAddress, const FString& Attributes)
{
	FString const Result = MapAddress + "?" + Attributes + "?ip=127.0.0.1 -log";

	/** it goes through all the directories and searches for the project folder (WindowsNoEditor->Project Name) */
	FString const URL = FPaths::ProjectDir() + "Binaries/Win64/SpaceWarServer.exe";

	/** work on windows dedicated */
	FPlatformProcess::CreateProc(*URL, *Result, true, false, false, nullptr, 0, nullptr, nullptr);
}
