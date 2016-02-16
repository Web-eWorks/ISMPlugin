// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// Created by Axtel Sturnclaw.

namespace UnrealBuildTool.Rules
{
	public class ISMPlugin : ModuleRules
	{
		public ISMPlugin(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"Developer/ISMPlugin/Private",
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
          "Engine",
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd",
					"Slate",
					"InputCore",
					"LevelEditor",
					"EditorStyle",
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}
