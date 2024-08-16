// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/HappyHazardGameMode.h"
#include "Character/HappyHazardCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHappyHazardGameMode::AHappyHazardGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
