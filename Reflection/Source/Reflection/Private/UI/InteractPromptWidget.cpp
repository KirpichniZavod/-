#include "UI/InteractPromptWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UInteractPromptWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!PromptTextWidget)
    {
        PromptTextWidget = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("PromptText"));
        if (PromptTextWidget)
        {
            PromptTextWidget->SetJustification(ETextJustify::Center);
            PromptTextWidget->SetText(FText::FromString(TEXT("Нажмите E, чтобы взаимодействовать")));
            UCanvasPanel* Root = Cast<UCanvasPanel>(GetRootWidget());
            if (!Root)
            {
                Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
                WidgetTree->RootWidget = Root;
            }
            if (Root)
            {
                UCanvasPanelSlot* Slot = Root->AddChildToCanvas(PromptTextWidget);
                if (Slot)
                {
                    Slot->SetAnchors(FAnchors(0.5f, 0.9f, 0.5f, 0.9f));
                    Slot->SetAlignment(FVector2D(0.5f, 0.5f));
                    Slot->SetAutoSize(true);
                }
            }
        }
    }
}

void UInteractPromptWidget::SetPromptText(const FText& InText)
{
    if (PromptTextWidget)
    {
        PromptTextWidget->SetText(InText);
    }
}