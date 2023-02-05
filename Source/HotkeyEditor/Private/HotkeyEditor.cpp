// Copyright Epic Games, Inc. All Rights Reserved.

#include "HotkeyEditor.h"
#include "HotkeyEditorStyle.h"
#include "HotkeyEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widgets/SHotkeyCommandsView.h"
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
	CollectContexts();
	
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
					HotkeyCommandsView.ToSharedRef()
				]
				+SSplitter::Slot()
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::Get().GetBrush("ColorPicker.MultipleValuesBackground"))
				]
			]
		];
}

void FHotkeyEditorModule::CollectContexts()
{
	FInputBindingManager::Get().GetKnownInputContexts(Contexts);

	//Rebuild our commands view with our new terms
	HotkeyCommandsView.Reset();
	HotkeyCommandsView = SNew(SHotkeyCommandsView).InContextListItems(Contexts);
}

void FHotkeyEditorModule::OnSearchChanged(const FText& Filter)
{
	CollectContexts();
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