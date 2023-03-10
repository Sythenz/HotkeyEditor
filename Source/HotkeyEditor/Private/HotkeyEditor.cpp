// Copyright Epic Games, Inc. All Rights Reserved.

#include "HotkeyEditor.h"
#include "HotkeyEditorStyle.h"
#include "HotkeyEditorCommands.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "KeyboardLayoutSettings.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Widgets/SHotkeyCommandsView.h"
#include "Widgets/SKeyboardUI.h"
#include "Widgets/Input/SSearchBox.h"

static const FName HotkeyEditorTabName("Hotkey Editor");

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
		.SetDisplayName(LOCTEXT("FHotkeyEditorTabTitle", "Hotkey Editor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	RegisterSettings();
}

void FHotkeyEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FHotkeyEditorStyle::Shutdown();

	FHotkeyEditorCommands::Unregister();

	DeregisterSettings();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(HotkeyEditorTabName);
}

void FHotkeyEditorModule::RegisterSettings()
{
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "HotkeyEditor",
			NSLOCTEXT("HotkeyEditor", "HotkeyEditorSettingsName", "Hotkey Editor"),
			NSLOCTEXT("HotkeyEditor", "HotkeyEditorDescription", "Configure Hotkey Editor Layouts"),
			GetMutableDefault<UKeyboardLayoutSettings>());
	}
}

void FHotkeyEditorModule::DeregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "HotkeyEditor");
	}
}

TSharedRef<SDockTab> FHotkeyEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10.0f, 10.0f, 10.0f, 0.0f)
			[
				SNew(SSearchBox)
				.OnTextChanged_Raw(this, &FHotkeyEditorModule::OnSearchChanged)
			]
			+SVerticalBox::Slot()
			.FillHeight(1.0f)
			.Padding(10.0f)
			[
				SNew(SSplitter)
				.Orientation(Orient_Vertical)
				+SSplitter::Slot()
				[
					SAssignNew(HotkeyCommandsView, SHotkeyCommandsView)
				]
				+SSplitter::Slot()
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::Get().GetBrush("ColorPicker.MultipleValuesBackground"))
					.Padding(10.0f)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SKeyboardUI)
					]
				]
			]
		];
}

void FHotkeyEditorModule::OnSearchChanged(const FText& Text)
{
	if(HotkeyCommandsView.IsValid())
	{
		HotkeyCommandsView->FilterContextsBySearch(Text);
	}
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