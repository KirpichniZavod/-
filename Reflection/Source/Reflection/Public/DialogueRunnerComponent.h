#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueRunnerComponent.generated.h"

class UDialogueDataAsset;
struct FDialogueLine;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueLineChanged, const FText&, Speaker, const FText&, Text, const TArray<FText>&, Choices);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REFLECTION_API UDialogueRunnerComponent : public UActorComponent
{
    GENERATED_BODY()

public: 
    UPROPERTY(BlueprintAssignable)
    FOnDialogueLineChanged OnLineChanged;

    UPROPERTY(BlueprintAssignable)
    FOnDialogueEnded OnDialogueEnded;

    UFUNCTION(BlueprintCallable)
    void StartDialogue(UDialogueDataAsset* Dialogue, int32 StartIndex = 0);

    UFUNCTION(BlueprintCallable)
    void ChooseOption(int32 ChoiceIndex);

    UFUNCTION(BlueprintCallable)
    void EndDialogue();

protected:
    UPROPERTY(Transient)
    UDialogueDataAsset* ActiveDialogue;

    int32 CurrentIndex = INDEX_NONE;

    void Emit();
};