// Copyright Epic Games, Inc. All Rights Reserved.

#include "HotkeyEditorCommands.h"

#define LOCTEXT_NAMESPACE "FHotkeyEditorModule"

void FHotkeyEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "HotkeyEditor", "Bring up HotkeyEditor window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
