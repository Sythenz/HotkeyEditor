// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KeyboardLayoutSettings.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class HOTKEYEDITOR_API SKeyboardUI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SKeyboardUI)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void PopulateKeyWidgets(TArray<FKeyboardRow> Keyboard, TSharedPtr<SVerticalBox> KeyboardWidget) const;

	TSharedPtr<SVerticalBox> LeftWidget;
	TSharedPtr<SVerticalBox> CenterWidget;
	TSharedPtr<SVerticalBox> RightWidget;

	UKeyboardLayoutSettings* KeyboardSettings;
};
