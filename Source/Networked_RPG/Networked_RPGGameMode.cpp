// Copyright Epic Games, Inc. All Rights Reserved.

#include "Networked_RPGGameMode.h"
#include "NDDCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANetworked_RPGGameMode::ANetworked_RPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
