// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ISMPluginPrivatePCH.h"
#include "ModuleManager.h"
#include "ISMActions.h"
#include "LevelEditor.h"

class FISMPlugin : public IISMPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FISMPlugin, ISMPlugin )



void FISMPlugin::StartupModule()
{
	FISMActions::Register();
	FUICommandList& ActionList = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor").GetGlobalLevelEditorActions().Get();
	const FISMActions& Actions = FISMActions::Get();
	ActionList.MapAction( Actions.CreateInstancedStaticMeshFromSelection, FExecuteAction::CreateStatic( &FISMActions::OnCreateInstancedStaticMeshFromSelection ) );
	ActionList.MapAction( Actions.RevertInstancedStaticMesh, FExecuteAction::CreateStatic( &FISMActions::OnRevertInstancedStaticMesh ) );
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FISMPlugin::ShutdownModule()
{
	FISMActions::Unregister();
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}
