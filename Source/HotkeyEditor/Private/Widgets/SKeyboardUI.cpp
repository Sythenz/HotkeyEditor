// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SKeyboardUI.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SKeyboardUI::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
