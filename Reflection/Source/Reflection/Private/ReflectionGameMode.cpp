#include "ReflectionGameMode.h"
#include "ReflectionCharacter.h"
#include "ReflectionPlayerController.h"

AReflectionGameMode::AReflectionGameMode()
{
    DefaultPawnClass = AReflectionCharacter::StaticClass();
    PlayerControllerClass = AReflectionPlayerController::StaticClass();
}