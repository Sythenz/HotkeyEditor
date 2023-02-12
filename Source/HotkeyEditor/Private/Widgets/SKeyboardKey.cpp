// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SKeyboardKey.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SKeyboardKey::Construct(const FArguments& InArgs)
{
	Character = InArgs._Character;
	bIsEnabled = InArgs._isEnabled;
	ButtonTooltip = InArgs._ButtonTooltip;

	TSharedPtr<SBorder> Border = SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("ToolPanel.DarkGroupBorder"))
		.BorderBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f));

	if(bIsEnabled)
	{
		Border->SetContent(
			SNew(SButton)
			.ContentPadding(1.0f)
			.ToolTipText(ButtonTooltip)
			[
				SNew(SBox)
				.MinDesiredWidth(75.0f)
				.MinDesiredHeight(60.0f)
				[
					SNew(STextBlock)
					.Text(Character)
					.Font(FCoreStyle::Get().GetFontStyle("SmallFont"))
					.WrappingPolicy(ETextWrappingPolicy::DefaultWrapping)
					.AutoWrapText(true)
				]
			]
		);
	}
	
	ChildSlot
	[
		Border.ToSharedRef()
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
