// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// Created by Axtel Sturnclaw.

#pragma once
#include "SlateBasics.h"
#include "EditorStyle.h"

class FISMActions : public TCommands<FISMActions>
{
public:

  FISMActions() : TCommands<FISMActions>
	(
		"ISMPlugin", // Context name for fast lookup
		NSLOCTEXT("Contexts", "ISMPlugin", "ISM Plugin"), // Localized context name for displaying
		NAME_None, // Parent
		FEditorStyle::GetStyleSetName() // Icon Style Set
	)
	{ }

  /**
   * Initialize commands.
   */
  virtual void RegisterCommands() override;

public:

  TSharedPtr<FUICommandInfo> CreateInstancedStaticMeshFromSelection;
  TSharedPtr<FUICommandInfo> RevertInstancedStaticMesh;

  static void OnCreateInstancedStaticMeshFromSelection();
  static void OnRevertInstancedStaticMesh();

};
