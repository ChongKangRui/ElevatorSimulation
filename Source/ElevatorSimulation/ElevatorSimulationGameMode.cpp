// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElevatorSimulationGameMode.h"
#include "ElevatorSimulationCharacter.h"
#include "UObject/ConstructorHelpers.h"

AElevatorSimulationGameMode::AElevatorSimulationGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
}
