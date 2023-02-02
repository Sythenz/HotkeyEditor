// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "HotkeyEditorStyle.h"

class FHotkeyEditorCommands : public TCommands<FHotkeyEditorCommands>
{
public:

	FHotkeyEditorCommands()
		: TCommands<FHotkeyEditorCommands>(TEXT("HotkeyEditor"), NSLOCTEXT("Contexts", "HotkeyEditor", "HotkeyEditor Plugin"), NAME_None, FHotkeyEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};