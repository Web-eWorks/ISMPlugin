// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// Created by Axtel Sturnclaw.

namespace UnrealBuildTool.Rules
{
	public class ISMPlugin : ModuleRules
	{
		public ISMPlugin(TargetInfo Target)
		{

			PrivateIncludePaths.Add("ISMPlugin/Private");

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
          "Engine",
					"ISMCore",
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
