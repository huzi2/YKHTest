// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "YKHProjectGameMode.h"
#include "YKHProjectPlayerController.h"
#include "YKHProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AYKHProjectGameMode::AYKHProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AYKHProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}