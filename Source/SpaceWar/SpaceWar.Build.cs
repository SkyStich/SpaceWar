// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpaceWar : ModuleRules
{
	public SpaceWar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "AIModule", "NavigationSystem", 
			"GameplayTags", "OnlineSubsystem", "OnlineSubsystemUtils"
		});
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Http", "Json", "JsonUtilities"
		});
	}
}
