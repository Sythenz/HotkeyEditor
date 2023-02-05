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
	SLATE_ARGUMENT(TArray<TSharedPtr<FBindingContext>>, InContextListItems)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	TSharedPtr<SListView<TSharedPtr<FBindingContext>>> ContextViewList;
	TSharedPtr<SListView<TSharedPtr<FUICommandInfo>>> CommandsViewList;

	TArray<TSharedPtr<FBindingContext>> ContextsListItems;
	TArray<TSharedPtr<FUICommandInfo>> CommandsListItems;

	TSharedRef<ITableRow> MakeContextsListRow(TSharedPtr<FBindingContext> Item, const TSharedRef<STableViewBase>& OwnerTable);
	TSharedRef<ITableRow> MakeCommandsListRow(TSharedPtr<FUICommandInfo> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void UpdateCommandsListFromContext(TSharedPtr<FBindingContext> InSelectedItem, ESelectInfo::Type SelectInfo);
	void UpdateDescriptionFromCommands(TSharedPtr<FUICommandInfo> InSelectedItem, ESelectInfo::Type SelectInfo);
	
	TSharedPtr<STextBlock> DescriptionTextBlock;
};
