// Copyright 2024, Maksym Kapelianovych. Licensed under MIT license.

using UnrealBuildTool;

public class TypedGameplayTags : ModuleRules
{
	public TypedGameplayTags(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "GameplayTags",
			}
			);
	}
}
