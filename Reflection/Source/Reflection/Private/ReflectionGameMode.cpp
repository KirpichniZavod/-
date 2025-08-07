#include "ReflectionGameMode.h"
#include "ReflectionCharacter.h"
#include "GameFramework/PlayerController.h"

AReflectionGameMode::AReflectionGameMode()
{
    DefaultPawnClass = AReflectionCharacter::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();
}