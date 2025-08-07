#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(BlueprintType)
class REFLECTION_API UInteractable : public UInterface
{
    GENERATED_BODY()
};

class REFLECTION_API IInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
    void Interact(AActor* InstigatorActor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
    FText GetDisplayName() const;
};