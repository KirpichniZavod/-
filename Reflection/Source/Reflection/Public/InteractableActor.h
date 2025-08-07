#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractableActor.generated.h"

UCLASS()
class REFLECTION_API AInteractableActor : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    AInteractableActor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
    FText DisplayName;

public:
    virtual void Interact_Implementation(AActor* InstigatorActor) override;
    virtual FText GetDisplayName_Implementation() const override { return DisplayName; }
};