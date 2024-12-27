// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FilteredGameplayTags : ModuleRules
{
	public FilteredGameplayTags(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
	}
}
