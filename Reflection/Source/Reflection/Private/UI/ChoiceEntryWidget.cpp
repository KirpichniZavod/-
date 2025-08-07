#include "UI/ChoiceEntryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBoxSlot.h"

void UChoiceEntryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!Button)
    {
        Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ChoiceButton"));
        WidgetTree->RootWidget = Button;
        Label = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Label"));
        Label->SetAutoWrapText(true);
        Button->AddChild(Label);
        Button->OnClicked.AddDynamic(this, &UChoiceEntryWidget::HandleClicked);
    }
}

void UChoiceEntryWidget::SetText(const FText& InText)
{
    if (Label)
    {
        Label->SetText(InText);
    }
}

void UChoiceEntryWidget::HandleClicked()
{
    OnSelected.Broadcast(Index);
}