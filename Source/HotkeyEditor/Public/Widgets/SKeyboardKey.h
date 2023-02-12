// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class HOTKEYEDITOR_API SKeyboardKey : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SKeyboardKey)
		{
		}

	SLATE_ARGUMENT(FText, Character)
	SLATE_ARGUMENT(bool, isEnabled)
	SLATE_ARGUMENT(FText, ButtonTooltip)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/* Character to display on the button */
	FText Character = FText();

	/* If the key is enabled */
	bool bIsEnabled = false;

	/* Text displayed in the tooltip of this button (the key command assigned) */
	FText ButtonTooltip = FText();
};
