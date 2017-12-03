// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "YKHProjectGameMode.h"
#include "YKHProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AYKHProjectGameMode::AYKHProjectGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
