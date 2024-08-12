// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ElevatorSimulation : ModuleRules
{
	public ElevatorSimulation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
		 new string[] {
				"ElevatorSimulation"
		 }
		);

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
