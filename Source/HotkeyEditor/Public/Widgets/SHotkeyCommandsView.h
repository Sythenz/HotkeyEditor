// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class HOTKEYEDITOR_API SHotkeyCommandsView : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHotkeyCommandsView)
		{
		}
	SLATE_END_ARGS()
	
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void FilterContextsBySearch(FText InTerm);

	FString SearchTerm;

	/* Context items filtered by search term, this gets displayed in the contexts list. */
	TArray<TSharedPtr<FBindingContext>> FilteredContexts;

	TArray<TSharedPtr<FBindingContext>> Contexts;
	
	TSharedPtr<SListView<TSharedPtr<FBindingContext>>> ContextViewList;
	TSharedPtr<SListView<TSharedPtr<FUICommandInfo>>> CommandsViewList;
	
	/* Command items filtered by search term, this gets displayed in the contexts list. */
	TArray<TSharedPtr<FUICommandInfo>> FilteredCommands;
	
	TArray<TSharedPtr<FUICommandInfo>> Commands;
	
	TSharedRef<ITableRow> MakeContextsListRow(TSharedPtr<FBindingContext> Item, const TSharedRef<STableViewBase>& OwnerTable);
	TSharedRef<ITableRow> MakeCommandsListRow(TSharedPtr<FUICommandInfo> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void UpdateCommandsListFromContext(TSharedPtr<FBindingContext> InSelectedItem, ESelectInfo::Type SelectInfo);
	void UpdateDescriptionFromCommands(TSharedPtr<FUICommandInfo> InSelectedItem, ESelectInfo::Type SelectInfo);
	
	TSharedPtr<STextBlock> DescriptionTextBlock;
};
