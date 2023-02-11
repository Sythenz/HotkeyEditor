// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SKeyboardUI.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SKeyboardUI::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.FillWidth(0.45f)
		[
			SNew(SBorder)
		]
		+SHorizontalBox::Slot()
		.FillWidth(0.25f)
		[
			SNew(SBorder)
			//KeyboardLayout.CenterKeyboard.ToSharedRef()
		]
		+SHorizontalBox::Slot()
		.FillWidth(0.3f)
		[
			SNew(SBorder)
			//KeyboardLayout.RightKeyboard.ToSharedRef()
		]
	];
}

void SKeyboardUI::PopulateKeyWidgets()
{
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
