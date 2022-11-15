// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_InventoryGameMode.h"
#include "UE5_InventoryPlayerController.h"
#include "UE5_InventoryCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE5_InventoryGameMode::AUE5_InventoryGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUE5_InventoryPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}