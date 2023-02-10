// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SHotkeyCommandsView.h"

#include "DiffResults.h"
#include "SlateOptMacros.h"
#include "Serialization/JsonTypes.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SHotkeyCommandsView::Construct(const FArguments& InArgs)
{	
	ChildSlot
	[
		SNew(SSplitter)
		.Orientation(Orient_Horizontal)
		+SSplitter::Slot()
		[
			SAssignNew(ContextViewList, SListView<TSharedPtr<FBindingContext>>)
			.ItemHeight(24.0f)
			.ListItemsSource(&FilteredContexts)
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
			.ListItemsSource(&FilteredCommands)
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

	/* Trigger a rebuild of a full list of contexts */
	FilterContextsBySearch(FText::FromString(""));
}

void SHotkeyCommandsView::FilterContextsBySearch(FText InTerm)
{
	/* Collect full contexts list from input manager */
	FInputBindingManager::Get().GetKnownInputContexts(Contexts);

	SearchTerm = InTerm.ToString();

	if(InTerm.ToString().IsEmpty() || InTerm.ToString() == "")
	{
		FilteredContexts = Contexts;
		ContextViewList->RebuildList();
		return;
	}
	
	FilteredContexts.Empty();

	for(int i = 0; i < Contexts.Num(); i++)
	{
		if(Contexts[i]->GetContextName().ToString().Contains(InTerm.ToString()))
		{
			FilteredContexts.AddUnique(Contexts[i]);
		}
	}

	ContextViewList->RebuildList();
}

TSharedRef<ITableRow> SHotkeyCommandsView::MakeContextsListRow(TSharedPtr<FBindingContext> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	const FString CategoryName = FName::NameToDisplayString(Item.Get()->GetContextName().ToString(), false);
	
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
	const FString Command = Item.Get()->GetInputText().ToString();

	const FMargin HasCommand = (Command != "") ? FMargin(4.0f, 0.0f, 0.0f, 0.0f) : FMargin(0.0f, 0.0f, 0.0f, 0.0f);

	return
		SNew(STableRow< TSharedRef<FText> >, OwnerTable)
		.Padding(8.0f)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(FText::FromString(CommandName))
			]
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::Get().GetBrush("ExpandableArea.Border"))
				.BorderBackgroundColor(FLinearColor(0.4f, 0.4f, 0.4f, 1.0f))
				.Padding(8.0f, 8.0f)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SImage)
						.Image(FAppStyle::Get().GetBrush("GraphEditor.KeyEvent_16x"))
						.ColorAndOpacity(FLinearColor(0.05f, 0.05f, 0.05f, 1.0f))
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SBox)
						.Padding(HasCommand)
						[
							SNew(STextBlock).Text(FText::FromString(Command))
						]
					]
				]
			]
		];
}

void SHotkeyCommandsView::UpdateCommandsListFromContext(TSharedPtr<FBindingContext> InSelectedItem, ESelectInfo::Type SelectInfo)
{
	if(!InSelectedItem.IsValid()) return;

	FInputBindingManager::Get().GetCommandInfosFromContext(InSelectedItem.Get()->GetContextName(), Commands);

	if(SearchTerm != "")
	{
		FilteredCommands.Empty();
		
		for(int32 i = 0; i < Commands.Num(); i++)
		{
			if(Commands[i].Get()->GetCommandName().ToString().Contains(SearchTerm))
			{
				FilteredCommands.Add(Commands[i]);
			}
		}
	}
	else
	{
		FilteredCommands = Commands;
	}
	
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
