#include "DialogueRunnerComponent.h"
#include "DialogueTypes.h"

void UDialogueRunnerComponent::StartDialogue(UDialogueDataAsset* Dialogue, int32 StartIndex)
{
    ActiveDialogue = Dialogue;
    CurrentIndex = StartIndex;
    Emit();
}

void UDialogueRunnerComponent::ChooseOption(int32 ChoiceIndex)
{
    if (!ActiveDialogue || !ActiveDialogue->Lines.IsValidIndex(CurrentIndex))
    {
        EndDialogue();
        return;
    }

    const FDialogueLine& Line = ActiveDialogue->Lines[CurrentIndex];
    if (Line.Choices.Num() == 0)
    {
        EndDialogue();
        return;
    }

    if (!Line.Choices.IsValidIndex(ChoiceIndex))
    {
        EndDialogue();
        return;
    }

    const int32 NextIdx = Line.Choices[ChoiceIndex].NextLineIndex;
    if (NextIdx == INDEX_NONE)
    {
        EndDialogue();
        return;
    }

    CurrentIndex = NextIdx;
    Emit();
}

void UDialogueRunnerComponent::EndDialogue()
{
    ActiveDialogue = nullptr;
    CurrentIndex = INDEX_NONE;
    OnDialogueEnded.Broadcast();
}

void UDialogueRunnerComponent::Emit()
{
    if (!ActiveDialogue || !ActiveDialogue->Lines.IsValidIndex(CurrentIndex))
    {
        EndDialogue();
        return;
    }

    const FDialogueLine& Line = ActiveDialogue->Lines[CurrentIndex];
    TArray<FText> Choices;
    for (const auto& C : Line.Choices)
    {
        Choices.Add(C.Text);
    }
    OnLineChanged.Broadcast(Line.Speaker, Line.Text, Choices);
}