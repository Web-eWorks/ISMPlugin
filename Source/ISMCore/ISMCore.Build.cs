// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// Created by Axtel Sturnclaw.

namespace UnrealBuildTool.Rules
{
	public class ISMCore : ModuleRules
	{
		public ISMCore(TargetInfo Target)
		{
			PublicIncludePaths.Add("ISMCore/Public");
			PrivateIncludePaths.Add("ISMCore/Private");

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
          "Engine",
				}
			);
		}
	}
}
