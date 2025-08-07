#include "InteractableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"

AInteractableActor::AInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);
}

void AInteractableActor::Interact_Implementation(AActor* InstigatorActor)
{
#if !(UE_BUILD_SHIPPING)
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.5f,
            FColor::Yellow,
            FString::Printf(TEXT("Взаимодействие: %s"), *DisplayName.ToString())
        );
    }
#endif
}