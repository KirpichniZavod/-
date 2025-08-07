#include "UI/DialogueWidget.h"
#include "DialogueTypes.h"

void UDialogueWidget::OpenDialogue(UDialogueDataAsset* DialogueData, int32 StartIndex)
{
    ActiveDialogue = DialogueData;
    CurrentIndex = StartIndex;
    Refresh();
}

void UDialogueWidget::CloseDialogue()
{
    ActiveDialogue = nullptr;
    CurrentIndex = INDEX_NONE;
    BP_OnDialogueClosed();
}

void UDialogueWidget::ChooseOption(int32 ChoiceIndex)
{
    if (!ActiveDialogue || !ActiveDialogue->Lines.IsValidIndex(CurrentIndex))
    {
        CloseDialogue();
        return;
    }

    const FDialogueLine& Line = ActiveDialogue->Lines[CurrentIndex];

    if (Line.Choices.Num() == 0)
    {
        CloseDialogue();
        return;
    }

    if (!Line.Choices.IsValidIndex(ChoiceIndex))
    {
        CloseDialogue();
        return;
    }

    const int32 NextIdx = Line.Choices[ChoiceIndex].NextLineIndex;
    if (NextIdx == INDEX_NONE)
    {
        CloseDialogue();
        return;
    }

    CurrentIndex = NextIdx;
    Refresh();
}

void UDialogueWidget::Refresh()
{
    if (!ActiveDialogue || !ActiveDialogue->Lines.IsValidIndex(CurrentIndex))
    {
        CloseDialogue();
        return;
    }

    const FDialogueLine& Line = ActiveDialogue->Lines[CurrentIndex];

    TArray<FText> ChoiceTexts;
    for (const auto& Choice : Line.Choices)
    {
        ChoiceTexts.Add(Choice.Text);
    }

    BP_ShowLine(Line.Speaker, Line.Text, ChoiceTexts);
}