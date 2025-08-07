#include "ReflectionGameInstance.h"
#include "ReflectionSaveGame.h"
#include "Kismet/GameplayStatics.h"

bool UReflectionGameInstance::SaveToSlot(const FString& SlotName, int32 UserIndex)
{
    UReflectionSaveGame* Save = NewObject<UReflectionSaveGame>();
    return UGameplayStatics::SaveGameToSlot(Save, SlotName, UserIndex);
}

UReflectionSaveGame* UReflectionGameInstance::LoadFromSlot(const FString& SlotName, int32 UserIndex)
{
    USaveGame* Base = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
    return Cast<UReflectionSaveGame>(Base);
}

void UReflectionGameInstance::TravelToLevelByName(const FName LevelName)
{
    if (!LevelName.IsNone())
    {
        UGameplayStatics::OpenLevel(GetWorld(), LevelName);
    }
}