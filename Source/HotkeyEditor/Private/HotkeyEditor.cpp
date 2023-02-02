// Copyright Epic Games, Inc. All Rights Reserved.

#include "HotkeyEditor.h"
#include "HotkeyEditorStyle.h"
#include "HotkeyEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName HotkeyEditorTabName("HotkeyEditor");

#define LOCTEXT_NAMESPACE "FHotkeyEditorModule"

void FHotkeyEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FHotkeyEditorStyle::Initialize();
	FHotkeyEditorStyle::ReloadTextures();

	FHotkeyEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FHotkeyEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FHotkeyEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FHotkeyEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(HotkeyEditorTabName, FOnSpawnTab::CreateRaw(this, &FHotkeyEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FHotkeyEditorTabTitle", "HotkeyEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FHotkeyEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FHotkeyEditorStyle::Shutdown();

	FHotkeyEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(HotkeyEditorTabName);
}

TSharedRef<SDockTab> FHotkeyEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FHotkeyEditorModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("HotkeyEditor.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FHotkeyEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(HotkeyEditorTabName);
}

void FHotkeyEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FHotkeyEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FHotkeyEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHotkeyEditorModule, HotkeyEditor)