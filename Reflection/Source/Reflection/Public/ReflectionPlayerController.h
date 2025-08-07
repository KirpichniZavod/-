#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ReflectionPlayerController.generated.h"

class UUserWidget;
class UInteractPromptWidget;
class UPauseMenuWidget;
class UDialogueWidget;
class UDialogueDataAsset;

UCLASS()
class REFLECTION_API AReflectionPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AReflectionPlayerController();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void TogglePauseMenu();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowPauseMenu();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HidePauseMenu();

    UFUNCTION()
    void HandleQuitRequested();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowInteractPrompt(const FText& PromptText);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideInteractPrompt();

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void StartDialogue(UDialogueDataAsset* DialogueData, int32 StartIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void EndDialogue();

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void ChooseDialogueOption(int32 ChoiceIndex);

protected:
    virtual void BeginPlay() override;

    void ApplyPauseInputMode(bool bPaused);

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> PauseMenuClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> InteractPromptClass;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> DialogueWidgetClass;

    UPROPERTY()
    UPauseMenuWidget* PauseMenuWidgetInstance;

    UPROPERTY()
    UInteractPromptWidget* InteractPromptWidgetInstance;

    UPROPERTY()
    UDialogueWidget* DialogueWidgetInstance;

    bool bPauseMenuVisible;
};