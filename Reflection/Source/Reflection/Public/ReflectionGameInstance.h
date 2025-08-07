#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ReflectionGameInstance.generated.h"

UCLASS()
class REFLECTION_API UReflectionGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Save")
    bool SaveToSlot(const FString& SlotName, int32 UserIndex);

    UFUNCTION(BlueprintCallable, Category = "Save")
    class UReflectionSaveGame* LoadFromSlot(const FString& SlotName, int32 UserIndex);

    UFUNCTION(BlueprintCallable, Category = "Level")
    void TravelToLevelByName(const FName LevelName);
};