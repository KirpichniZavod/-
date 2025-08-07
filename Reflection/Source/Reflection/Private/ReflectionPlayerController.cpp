#include "ReflectionPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/InteractPromptWidget.h"
#include "UI/PauseMenuWidget.h"
#include "UI/DialogueWidget.h"
#include "DialogueTypes.h"
#include "Kismet/KismetSystemLibrary.h"

AReflectionPlayerController::AReflectionPlayerController()
{
    bPauseMenuVisible = false;
    bShowMouseCursor = false;
}

void AReflectionPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (InteractPromptClass)
    {
        UUserWidget* Widget = CreateWidget(this, InteractPromptClass);
        InteractPromptWidgetInstance = Cast<UInteractPromptWidget>(Widget);
        if (InteractPromptWidgetInstance)
        {
            InteractPromptWidgetInstance->AddToViewport(5);
            InteractPromptWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void AReflectionPlayerController::ApplyPauseInputMode(bool bPaused)
{
    if (bPaused)
    {
        FInputModeUIOnly Mode;
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(Mode);
        bShowMouseCursor = true;
    }
    else
    {
        FInputModeGameOnly Mode;
        SetInputMode(Mode);
        bShowMouseCursor = false;
    }
}

void AReflectionPlayerController::TogglePauseMenu()
{
    if (bPauseMenuVisible) HidePauseMenu(); else ShowPauseMenu();
}

void AReflectionPlayerController::ShowPauseMenu()
{
    if (!PauseMenuWidgetInstance && PauseMenuClass)
    {
        UUserWidget* Widget = CreateWidget(this, PauseMenuClass);
        PauseMenuWidgetInstance = Cast<UPauseMenuWidget>(Widget);
        if (PauseMenuWidgetInstance)
        {
            PauseMenuWidgetInstance->OnResumeRequested.AddDynamic(this, &AReflectionPlayerController::HidePauseMenu);
            PauseMenuWidgetInstance->OnQuitRequested.AddDynamic(this, &AReflectionPlayerController::HandleQuitRequested);
        }
    }

    if (PauseMenuWidgetInstance && !bPauseMenuVisible)
    {
        PauseMenuWidgetInstance->AddToViewport(10);
        PauseMenuWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        SetPause(true);
        ApplyPauseInputMode(true);
        bPauseMenuVisible = true;
    }
}

void AReflectionPlayerController::HidePauseMenu()
{
    if (PauseMenuWidgetInstance && bPauseMenuVisible)
    {
        PauseMenuWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        PauseMenuWidgetInstance->RemoveFromParent();
        SetPause(false);
        ApplyPauseInputMode(false);
        bPauseMenuVisible = false;
    }
}

void AReflectionPlayerController::HandleQuitRequested()
{
    // Закрыть игру с подтверждением можно добавить позже
    UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}

void AReflectionPlayerController::ShowInteractPrompt(const FText& PromptText)
{
    if (InteractPromptWidgetInstance)
    {
        InteractPromptWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        InteractPromptWidgetInstance->SetPromptText(PromptText);
    }
}

void AReflectionPlayerController::HideInteractPrompt()
{
    if (InteractPromptWidgetInstance)
    {
        InteractPromptWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AReflectionPlayerController::StartDialogue(UDialogueDataAsset* DialogueData, int32 StartIndex)
{
    if (!DialogueWidgetInstance && DialogueWidgetClass)
    {
        UUserWidget* Widget = CreateWidget(this, DialogueWidgetClass);
        DialogueWidgetInstance = Cast<UDialogueWidget>(Widget);
    }

    if (DialogueWidgetInstance)
    {
        DialogueWidgetInstance->AddToViewport(15);
        DialogueWidgetInstance->OpenDialogue(DialogueData, StartIndex);

        FInputModeUIOnly Mode;
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(Mode);
        bShowMouseCursor = true;
    }
}

void AReflectionPlayerController::EndDialogue()
{
    if (DialogueWidgetInstance)
    {
        DialogueWidgetInstance->CloseDialogue();
        DialogueWidgetInstance->RemoveFromParent();

        FInputModeGameOnly Mode;
        SetInputMode(Mode);
        bShowMouseCursor = false;
    }
}

void AReflectionPlayerController::ChooseDialogueOption(int32 ChoiceIndex)
{
    if (DialogueWidgetInstance)
    {
        DialogueWidgetInstance->ChooseOption(ChoiceIndex);
    }
}