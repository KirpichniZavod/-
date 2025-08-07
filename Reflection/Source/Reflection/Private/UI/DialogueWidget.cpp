#include "UI/DialogueWidget.h"
#include "DialogueTypes.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/TextBlock.h"
#include "UI/ChoiceEntryWidget.h"

void UDialogueWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!RootBox)
    {
        RootBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Root"));
        WidgetTree->RootWidget = RootBox;

        SpeakerText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Speaker"));
        UVerticalBoxSlot* SSlot = RootBox->AddChildToVerticalBox(SpeakerText);
        if (SSlot) SSlot->SetPadding(FMargin(4.f));

        LineText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Line"));
        LineText->SetAutoWrapText(true);
        UVerticalBoxSlot* LSlot = RootBox->AddChildToVerticalBox(LineText);
        if (LSlot) LSlot->SetPadding(FMargin(4.f));

        ChoicesBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Choices"));
        UVerticalBoxSlot* CSlot = RootBox->AddChildToVerticalBox(ChoicesBox);
        if (CSlot) CSlot->SetPadding(FMargin(8.f));
    }
}

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
    RemoveFromParent();
}

void UDialogueWidget::ChooseOption(int32 ChoiceIndex)
{
    OnChoiceClicked(ChoiceIndex);
}

void UDialogueWidget::OnChoiceClicked(int32 Index)
{
    if (!ActiveDialogue || !ActiveDialogue->Lines.IsValidIndex(CurrentIndex))
    {
        CloseDialogue();
        return;
    }

    const FDialogueLine& Line = ActiveDialogue->Lines[CurrentIndex];
    if (!Line.Choices.IsValidIndex(Index))
    {
        CloseDialogue();
        return;
    }

    const int32 Next = Line.Choices[Index].NextLineIndex;
    if (Next == INDEX_NONE)
    {
        CloseDialogue();
        return;
    }

    CurrentIndex = Next;
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

    if (SpeakerText) SpeakerText->SetText(Line.Speaker);
    if (LineText) LineText->SetText(Line.Text);

    if (ChoicesBox)
    {
        ChoicesBox->ClearChildren();
        for (int32 i = 0; i < Line.Choices.Num(); ++i)
        {
            UChoiceEntryWidget* Entry = WidgetTree->ConstructWidget<UChoiceEntryWidget>(UChoiceEntryWidget::StaticClass());
            Entry->SetText(Line.Choices[i].Text);
            Entry->SetIndex(i);
            Entry->OnSelected.AddDynamic(this, &UDialogueWidget::ChooseOption);
            ChoicesBox->AddChildToVerticalBox(Entry);
        }
    }
}