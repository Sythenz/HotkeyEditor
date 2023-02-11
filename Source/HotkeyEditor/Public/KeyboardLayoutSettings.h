// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/DeveloperSettings.h"
#include "KeyboardLayoutSettings.generated.h"

USTRUCT()
struct FKeyboardKey
{
	GENERATED_BODY()

public:
		
	UPROPERTY(Config, EditAnywhere, Category = "General")
	bool bIsSpace = false;
		
	UPROPERTY(Config, EditAnywhere, Category = "General", meta=(EditCondition="!bIsSpace"))
	FString Character;
	
	UPROPERTY(Config, EditAnywhere, Category = "General")
	FVector2D Size = FVector2D(1.0f, 1.0f);
};

USTRUCT()
struct FKeyboardRow
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "General", meta=(EditCondition="!bIsEmptyRow"))
	TArray<FKeyboardKey> KeyboardRow;
	
	UPROPERTY(Config, EditAnywhere, Category = "General")
	bool bIsEmptyRow = false;
};

USTRUCT()
struct FKeyboardLayout
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "General")
	TArray<FKeyboardRow> LeftKeyboard;
	
	UPROPERTY(Config, EditAnywhere, Category = "General")
	TArray<FKeyboardRow> CenterKeyboard;

	UPROPERTY(Config, EditAnywhere, Category = "General")
	TArray<FKeyboardRow> RightKeyboard;
};

/**
 * 
 */
UCLASS(Config=EditorPerProjectUserSettings, DefaultConfig, meta =(DisplayName="Keyboard Layout Settings"))
class HOTKEYEDITOR_API UKeyboardLayoutSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "General")
	FKeyboardLayout KeyboardLayout;
};
