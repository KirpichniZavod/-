#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class UDialogueDataAsset;

UCLASS()
class REFLECTION_API UDialogueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OpenDialogue(UDialogueDataAsset* DialogueData, int32 StartIndex);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void CloseDialogue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void ChooseOption(int32 ChoiceIndex);

protected:
    UPROPERTY(Transient)
    UDialogueDataAsset* ActiveDialogue;

    int32 CurrentIndex = INDEX_NONE;

    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
    void BP_ShowLine(const FText& Speaker, const FText& Text, const TArray<FText>& Choices);

    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
    void BP_OnDialogueClosed();

    void Refresh();
};