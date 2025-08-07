#include "ReflectionLevelDirector.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "InteractableActor.h"
#include "ReflectionCharacter.h"

AReflectionLevelDirector::AReflectionLevelDirector()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AReflectionLevelDirector::BeginPlay()
{
    Super::BeginPlay();
    PlayerCharacter = Cast<AReflectionCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    BuildScene(EReflectionScene::Apartment_Morning);
}

void AReflectionLevelDirector::ClearSpawned()
{
    for (AActor* A : SpawnedActors)
    {
        if (IsValid(A)) A->Destroy();
    }
    SpawnedActors.Empty();
}

AActor* AReflectionLevelDirector::SpawnBox(const FVector& Location, const FVector& Scale, const FRotator& Rotation, const FLinearColor& Color, const FString& Name)
{
    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (!CubeMesh) return nullptr;

    AStaticMeshActor* Actor = GetWorld()->SpawnActor<AStaticMeshActor>(Location, Rotation);
    if (!Actor) return nullptr;

    UStaticMeshComponent* Comp = Actor->GetStaticMeshComponent();
    Comp->SetStaticMesh(CubeMesh);
    Comp->SetMobility(EComponentMobility::Static);
    Comp->SetWorldScale3D(Scale);

    UMaterialInterface* BaseMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (BaseMat)
    {
        UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseMat, Actor);
        MID->SetVectorParameterValue(TEXT("Color"), Color);
        Comp->SetMaterial(0, MID);
    }

    Actor->SetActorLabel(Name);
    SpawnedActors.Add(Actor);
    return Actor;
}

void AReflectionLevelDirector::PlaceInteractable(const FVector& Location, const FVector& Scale, const FText& Name, TFunction<void()> OnInteractLambda)
{
    AInteractableActor* Inter = GetWorld()->SpawnActor<AInteractableActor>(Location, FRotator::ZeroRotator);
    if (Inter)
    {
        Inter->SetActorScale3D(Scale);
        Inter->Tags.Add(FName(TEXT("LambdaInteract")));
        Inter->SetActorLabel(Name.ToString());
        Inter->Interact_Implementation = [Inter, OnInteractLambda](AActor*) mutable { OnInteractLambda(); };
        // Note: C++ cannot rebind virtual method at runtime; реальная логика делается через наследника/блюпринт.
        // Здесь оставляем как placeholder: взаимодействие покажет DisplayName.
        SpawnedActors.Add(Inter);
    }
}

void AReflectionLevelDirector::Build_Apartment(bool bVariant)
{
    ClearSpawned();

    // Комната
    SpawnBox(FVector(0,0,0), FVector(6,6,0.1f), FRotator::ZeroRotator, FLinearColor(0.8f,0.8f,0.8f), TEXT("Floor"));
    SpawnBox(FVector(0,-600,300), FVector(6,0.1f,3), FRotator::ZeroRotator, FLinearColor(0.9f,0.9f,0.9f), TEXT("Wall1"));
    SpawnBox(FVector(0,600,300), FVector(6,0.1f,3), FRotator::ZeroRotator, FLinearColor(0.9f,0.9f,0.9f), TEXT("Wall2"));
    SpawnBox(FVector(-600,0,300), FVector(0.1f,6,3), FRotator::ZeroRotator, FLinearColor(0.9f,0.9f,0.9f), TEXT("Wall3"));
    SpawnBox(FVector(600,0,300), FVector(0.1f,6,3), FRotator::ZeroRotator, FLinearColor(0.9f,0.9f,0.9f), TEXT("Wall4"));

    // Стол и кружка как интерактив
    SpawnBox(FVector(0,200,50), FVector(1.2f,0.8f,0.05f), FRotator::ZeroRotator, FLinearColor(0.6f,0.5f,0.4f), TEXT("Table"));
    SpawnBox(FVector(0,200,80), FVector(0.1f,0.1f,0.2f), FRotator::ZeroRotator, FLinearColor(0.9f,0.9f,1.0f), TEXT("Mug"));

    // "Зеркало" — просто панель
    SpawnBox(FVector(400,0,120), FVector(0.05f,1.2f,1.0f), FRotator::ZeroRotator, bVariant ? FLinearColor(0.6f,0.7f,0.7f) : FLinearColor(0.8f,0.9f,1.0f), TEXT("Mirror"));

    CurrentScene = bVariant ? EReflectionScene::Apartment_Variant : EReflectionScene::Apartment_Morning;
}

void AReflectionLevelDirector::Build_StairsStreet()
{
    ClearSpawned();
    // Лестничная клетка
    SpawnBox(FVector(0,0,0), FVector(6,6,0.1f), FRotator::ZeroRotator, FLinearColor::Gray, TEXT("Floor"));
    SpawnBox(FVector(0,600,300), FVector(6,0.1f,3), FRotator::ZeroRotator, FLinearColor::Gray, TEXT("ExitWall"));
    CurrentScene = EReflectionScene::Stairs_Street;
}

void AReflectionLevelDirector::Build_BusInterior()
{
    ClearSpawned();
    // Узкий коридор с лавками по бокам
    SpawnBox(FVector(0,0,0), FVector(12,2,0.1f), FRotator::ZeroRotator, FLinearColor(0.15f,0.15f,0.18f), TEXT("Floor"));
    SpawnBox(FVector(0,-200,50), FVector(12,0.1f,0.5f), FRotator::ZeroRotator, FLinearColor(0.2f,0.2f,0.25f), TEXT("BenchL"));
    SpawnBox(FVector(0,200,50), FVector(12,0.1f,0.5f), FRotator::ZeroRotator, FLinearColor(0.2f,0.2f,0.25f), TEXT("BenchR"));
    CurrentScene = EReflectionScene::Bus_Interior;
}

void AReflectionLevelDirector::Build_OfficeFloor()
{
    ClearSpawned();
    SpawnBox(FVector(0,0,0), FVector(10,10,0.1f), FRotator::ZeroRotator, FLinearColor(0.85f,0.85f,0.9f), TEXT("OfficeFloor"));
    // Столы
    for (int32 i=0;i<4;++i)
    {
        SpawnBox(FVector(-400 + i*250, -200, 50), FVector(1.2f,0.8f,0.05f), FRotator::ZeroRotator, FLinearColor(0.6f,0.5f,0.4f), FString::Printf(TEXT("Desk_%d"), i));
    }
    CurrentScene = EReflectionScene::Office_Floor;
}

void AReflectionLevelDirector::Build_Cafeteria()
{
    ClearSpawned();
    SpawnBox(FVector(0,0,0), FVector(10,10,0.1f), FRotator::ZeroRotator, FLinearColor(0.9f,0.9f,0.85f), TEXT("CafeFloor"));
    SpawnBox(FVector(0,0,50), FVector(1.5f,1.5f,0.05f), FRotator::ZeroRotator, FLinearColor(0.7f,0.7f,0.7f), TEXT("Table"));
    CurrentScene = EReflectionScene::Cafeteria;
}

void AReflectionLevelDirector::Build_StreetAccident()
{
    ClearSpawned();
    // Дорога
    SpawnBox(FVector(0,0,0), FVector(12,2,0.05f), FRotator::ZeroRotator, FLinearColor(0.05f,0.05f,0.05f), TEXT("Road"));
    // Заглушка машины — коробка, "вылетающая" на игрока
    AActor* Car = SpawnBox(FVector(-1500,0,50), FVector(1.8f,0.8f,0.4f), FRotator::ZeroRotator, FLinearColor(0.2f,0.2f,1.0f), TEXT("Car"));
    if (Car)
    {
        Car->SetActorEnableCollision(false);
        // Простейшее движение в тике LevelSequence тут не пишем. Предлагается в BP или Timelines доработать.
    }
    CurrentScene = EReflectionScene::Street_Accident;
}

void AReflectionLevelDirector::Build_Basement()
{
    ClearSpawned();
    SpawnBox(FVector(0,0,0), FVector(6,6,0.05f), FRotator::ZeroRotator, FLinearColor(0.1f,0.1f,0.1f), TEXT("Floor"));
    SpawnBox(FVector(0,-600,300), FVector(6,0.1f,3), FRotator::ZeroRotator, FLinearColor(0.12f,0.12f,0.12f), TEXT("Wall1"));
    SpawnBox(FVector(0,600,300), FVector(6,0.1f,3), FRotator::ZeroRotator, FLinearColor(0.12f,0.12f,0.12f), TEXT("Wall2"));
    SpawnBox(FVector(-600,0,300), FVector(0.1f,6,3), FRotator::ZeroRotator, FLinearColor(0.12f,0.12f,0.12f), TEXT("Wall3"));
    SpawnBox(FVector(600,0,300), FVector(0.1f,6,3), FRotator::ZeroRotator, FLinearColor(0.12f,0.12f,0.12f), TEXT("Wall4"));
    SpawnBox(FVector(0,0,80), FVector(1.0f,0.6f,0.05f), FRotator::ZeroRotator, FLinearColor(0.2f,0.2f,0.2f), TEXT("Table"));
    CurrentScene = EReflectionScene::Basement;
}

void AReflectionLevelDirector::BuildScene(EReflectionScene Scene)
{
    switch (Scene)
    {
    case EReflectionScene::Apartment_Morning: return Build_Apartment(false);
    case EReflectionScene::Stairs_Street: return Build_StairsStreet();
    case EReflectionScene::Bus_Interior: return Build_BusInterior();
    case EReflectionScene::Office_Floor: return Build_OfficeFloor();
    case EReflectionScene::Cafeteria: return Build_Cafeteria();
    case EReflectionScene::Street_Accident: return Build_StreetAccident();
    case EReflectionScene::Basement: return Build_Basement();
    case EReflectionScene::Apartment_Variant: return Build_Apartment(true);
    default: break;
    }
}

void AReflectionLevelDirector::AdvanceScene()
{
    const int32 Next = (static_cast<int32>(CurrentScene) + 1) % 8;
    BuildScene(static_cast<EReflectionScene>(Next));
}