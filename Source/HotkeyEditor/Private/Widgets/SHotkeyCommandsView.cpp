// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SHotkeyCommandsView.h"

#include "DiffResults.h"
#include "SlateOptMacros.h"
#include "Serialization/JsonTypes.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SHotkeyCommandsView::Construct(const FArguments& InArgs)
{
	ContextsListItems = InArgs._InContextListItems;
	
	ChildSlot
	[
		SNew(SSplitter)
		.Orientation(Orient_Horizontal)
		+SSplitter::Slot()
		[
			SAssignNew(ContextViewList, SListView<TSharedPtr<FBindingContext>>)
			.ItemHeight(24.0f)
			.ListItemsSource(&ContextsListItems)
			.OnGenerateRow(this, &SHotkeyCommandsView::MakeContextsListRow)
			.OnSelectionChanged(this, &SHotkeyCommandsView::UpdateCommandsListFromContext)
			.HeaderRow(
				SNew(SHeaderRow)
				+SHeaderRow::Column("Categories").DefaultLabel(FText::FromString("Categories"))
			)
		]
		+SSplitter::Slot()
		[
			SAssignNew(CommandsViewList, SListView<TSharedPtr<FUICommandInfo>>)
			.ItemHeight(24.0f)
			.ListItemsSource(&CommandsListItems)
			.OnGenerateRow(this, &SHotkeyCommandsView::MakeCommandsListRow)
			.OnSelectionChanged(this, &SHotkeyCommandsView::UpdateDescriptionFromCommands)
			.HeaderRow(
				SNew(SHeaderRow)
				+SHeaderRow::Column("Commands").DefaultLabel(FText::FromString("Commands"))
			)
		]
		+SSplitter::Slot()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHeaderRow)
				+SHeaderRow::Column("Description").DefaultLabel(FText::FromString("Description"))
			]
			+SVerticalBox::Slot()
			.FillHeight(1.0f)
			.Padding(5.0f)
			[
				SAssignNew(DescriptionTextBlock, STextBlock)
				.Text(FText::FromString("..."))
			]
							
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10.0f)
			.MaxHeight(180.0f)
			.VAlign(VAlign_Bottom)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("DashedBorder"))
				.BorderBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.1f))
				.Padding(15.0f)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(5.0f)
					.MaxWidth(72.0f)
					[
						SNew(SButton)
						[
							SNew(STextBlock)
							.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
							.AutoWrapText(true)
							.Text(FText::FromString("Set Hotkey"))
							.Font(FAppStyle::Get().GetFontStyle("SmallFont"))
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SBorder)
					]
				]
			]
		]
	];
}

TSharedRef<ITableRow> SHotkeyCommandsView::MakeContextsListRow(TSharedPtr<FBindingContext> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	const FString CategoryName = FName::NameToDisplayString(Item.Get()->GetContextName().ToString(), false);

	if(!Item.IsValid())
	{
		return SNew(STableRow< TSharedRef<FText> >, OwnerTable)
		.Padding(8.0f)
		[
			SNew(STextBlock).Text(FText::FromString(""))
		]; 
	}
	
	return
		SNew(STableRow< TSharedRef<FText> >, OwnerTable)
		.Padding(8.0f)
		[
			SNew(STextBlock).Text(FText::FromString(CategoryName))
		];
}

TSharedRef<ITableRow> SHotkeyCommandsView::MakeCommandsListRow(TSharedPtr<FUICommandInfo> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	const FString CommandName = FName::NameToDisplayString(Item.Get()->GetCommandName().ToString(), false);

	if(!Item.IsValid())
	{
		return SNew(STableRow< TSharedRef<FText> >, OwnerTable)
		.Padding(8.0f)
		[
			SNew(STextBlock).Text(FText::FromString(""))
		]; 
	}
	
	return
		SNew(STableRow< TSharedRef<FText> >, OwnerTable)
		.Padding(8.0f)
		[
			SNew(STextBlock).Text(FText::FromString(CommandName))
		];
}

void SHotkeyCommandsView::UpdateCommandsListFromContext(TSharedPtr<FBindingContext> InSelectedItem, ESelectInfo::Type SelectInfo)
{
	if(!InSelectedItem.IsValid()) return;

	FInputBindingManager::Get().GetCommandInfosFromContext(InSelectedItem.Get()->GetContextName(), CommandsListItems);
	CommandsViewList->RebuildList();
}

void SHotkeyCommandsView::UpdateDescriptionFromCommands(TSharedPtr<FUICommandInfo> InSelectedItem, ESelectInfo::Type SelectInfo)
{
	if(!InSelectedItem.IsValid())
	{
		DescriptionTextBlock->SetText(FText::FromString("..."));
	}
	else
	{
		DescriptionTextBlock->SetText(InSelectedItem->GetDescription());
	}
		CommandsViewList->RebuildList();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
