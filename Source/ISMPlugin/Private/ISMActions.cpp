// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// Created by Axtel Sturnclaw.

#include "ISMPluginPrivatePCH.h"
#include "ISMActions.h"
#include "ISMCreator.h"

#define LOCTEXT_NAMESPACE "LevelEditorActions"

void FISMActions::OnCreateInstancedStaticMeshFromSelection()
{
	FISMCreator::CreateInstancedStaticMeshFromSelection();
}
void FISMActions::OnRevertInstancedStaticMesh()
{
	FISMCreator::RevertSelectedInstancedStaticMeshes();
}

PRAGMA_DISABLE_OPTIMIZATION
void FISMActions::RegisterCommands()
{
  UI_COMMAND( CreateInstancedStaticMeshFromSelection, "Create Instanced Static Mesh From Selection",
  "When the key binding is pressed all selected static mesh actors with the same static mesh asset will be merged into a single high-performance instanced static mesh actor.",
  EUserInterfaceActionType::ToggleButton, FInputGesture(EModifierKey::Control, EKeys::I) );
  UI_COMMAND( RevertInstancedStaticMesh, "Revert Instanced Static Mesh",
  "When the key binding is pressed, the selected instanced static mesh will be turned back into editable static mesh actors.",
    EUserInterfaceActionType::ToggleButton, FInputGesture(EModifierKey::Shift|EModifierKey::Control, EKeys::I) );
}
PRAGMA_ENABLE_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
