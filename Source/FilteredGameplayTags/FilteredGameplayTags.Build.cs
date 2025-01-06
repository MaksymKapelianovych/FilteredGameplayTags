// Copyright 2024, Maksym Kapelianovych. Licensed under MIT license.

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
