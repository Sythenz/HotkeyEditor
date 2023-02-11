// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class SHotkeyCommandsView;
class FToolBarBuilder;
class FMenuBuilder;

class FHotkeyEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void RegisterSettings();
	void DeregisterSettings();

	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	void OnSearchChanged(const FText& Text);

	TArray<TSharedPtr<FBindingContext>> Contexts;
	TSharedPtr<SHotkeyCommandsView> HotkeyCommandsView;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
