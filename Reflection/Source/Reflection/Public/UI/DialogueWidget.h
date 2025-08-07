#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class UButton;
class UDialogueDataAsset;

UCLASS()
class REFLECTION_API UDialogueWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

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

    UPROPERTY(Transient)
    UVerticalBox* RootBox;

    UPROPERTY(Transient)
    UTextBlock* SpeakerText;

    UPROPERTY(Transient)
    UTextBlock* LineText;

    UPROPERTY(Transient)
    UVerticalBox* ChoicesBox;

    UFUNCTION()
    void OnChoiceClicked(int32 Index);

    void Refresh();
};