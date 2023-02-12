// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SKeyboardUI.h"
#include "Widgets/SKeyboardKey.h"

#include "SlateOptMacros.h"
#include "StaticMeshAttributes.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SKeyboardUI::Construct(const FArguments& InArgs)
{
	KeyboardSettings = GetMutableDefault<UKeyboardLayoutSettings>();
	
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("ToolPanel.DarkGroupBorder"))
		.BorderBackgroundColor(FLinearColor(0.35f, 0.35f, 0.35f, 1.0f))
		[
			SNew(SBox)
			.MaxDesiredWidth(1400.0f)
			.MaxDesiredHeight(300.0f)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(0.65f)
				.Padding(5.0f)
				[
					SAssignNew(LeftWidget, SVerticalBox)
				]
				+SHorizontalBox::Slot()
				.FillWidth(0.15f)
				.Padding(5.0f)
				[
					SAssignNew(CenterWidget, SVerticalBox)
				]
				+SHorizontalBox::Slot()
				.FillWidth(0.20f)
				.Padding(5.0f)
				[
					SAssignNew(RightWidget, SVerticalBox)
				]			
			]
		]
	];
	
	PopulateKeyWidgets(KeyboardSettings->KeyboardLayout.LeftKeyboard, LeftWidget);
	PopulateKeyWidgets(KeyboardSettings->KeyboardLayout.CenterKeyboard, CenterWidget);
	PopulateKeyWidgets(KeyboardSettings->KeyboardLayout.RightKeyboard, RightWidget);
}

void SKeyboardUI::PopulateKeyWidgets(TArray<FKeyboardRow> Keyboard, TSharedPtr<SVerticalBox> KeyboardWidget) const
{
	for(int i = 0; i < Keyboard.Num(); i++)
	{
		FKeyboardRow Row = Keyboard[i];
		TArray<FKeyboardKey> Keys = Row.KeyboardRow;
		
		TSharedPtr<SHorizontalBox> NewRow = SNew(SHorizontalBox);

		for(int j = 0; j < Keys.Num(); j++)
		{
			NewRow->AddSlot()
			.FillWidth(Row.KeyboardRow[j].Size.X)
			.Padding(3.0f)
			[
				SNew(SKeyboardKey)
				.Character(FText::FromString(Row.KeyboardRow[j].Character))
				.isEnabled(!Row.KeyboardRow[j].bIsSpace)
				.ButtonTooltip(FText::FromString("Test Tooltip"))
			];
		}
		
		KeyboardWidget->AddSlot()
		[
			NewRow.ToSharedRef()
		];
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
