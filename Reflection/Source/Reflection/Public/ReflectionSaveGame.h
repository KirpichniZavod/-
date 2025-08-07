#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ReflectionSaveGame.generated.h"

UCLASS()
class REFLECTION_API UReflectionSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "Save")
    FName CurrentSceneId;

    UPROPERTY(BlueprintReadWrite, Category = "Save")
    TMap<FName, bool> Flags;

    UReflectionSaveGame();
};