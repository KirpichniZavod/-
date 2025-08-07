#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReflectionLevelDirector.generated.h"

UENUM(BlueprintType)
enum class EReflectionScene : uint8
{
    Apartment_Morning = 0,
    Stairs_Street     = 1,
    Bus_Interior      = 2,
    Office_Floor      = 3,
    Cafeteria         = 4,
    Street_Accident   = 5,
    Basement          = 6,
    Apartment_Variant = 7
};

UCLASS()
class REFLECTION_API AReflectionLevelDirector : public AActor
{
    GENERATED_BODY()

public:
    AReflectionLevelDirector();

    UFUNCTION(BlueprintCallable)
    void BuildScene(EReflectionScene Scene);

    UFUNCTION(BlueprintCallable)
    void AdvanceScene();

protected:
    virtual void BeginPlay() override;

    void ClearSpawned();

    void Build_Apartment(bool bVariant);
    void Build_StairsStreet();
    void Build_BusInterior();
    void Build_OfficeFloor();
    void Build_Cafeteria();
    void Build_StreetAccident();
    void Build_Basement();

    AActor* SpawnBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation = FRotator::ZeroRotator, const FLinearColor& Color = FLinearColor::Gray, const FString& Name = TEXT("Box"));

    void PlaceInteractable(const FVector& Location, const FVector& Scale, const FText& Name, TFunction<void()> OnInteractLambda);

    void SetupAccidentCar();

    UPROPERTY()
    TArray<AActor*> SpawnedActors;

    EReflectionScene CurrentScene;

    TWeakObjectPtr<class AReflectionCharacter> PlayerCharacter;
};