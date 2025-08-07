#include "UI/PauseMenuWidget.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!RootBox)
    {
        RootBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Root"));
        WidgetTree->RootWidget = RootBox;

        // Resume
        ResumeButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ResumeButton"));
        UVerticalBoxSlot* ResumeSlot = RootBox->AddChildToVerticalBox(ResumeButton);
        if (ResumeSlot)
        {
            ResumeSlot->SetPadding(FMargin(8.f));
        }
        UTextBlock* ResumeText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ResumeText"));
        ResumeText->SetText(FText::FromString(TEXT("Продолжить")));
        ResumeButton->AddChild(ResumeText);
        ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);

        // Quit
        QuitButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("QuitButton"));
        UVerticalBoxSlot* QuitSlot = RootBox->AddChildToVerticalBox(QuitButton);
        if (QuitSlot)
        {
            QuitSlot->SetPadding(FMargin(8.f));
        }
        UTextBlock* QuitText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("QuitText"));
        QuitText->SetText(FText::FromString(TEXT("Выйти")));
        QuitButton->AddChild(QuitText);
        QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
    }
}

void UPauseMenuWidget::OnResumeClicked()
{
    OnResumeRequested.Broadcast();
}

void UPauseMenuWidget::OnQuitClicked()
{
    OnQuitRequested.Broadcast();
}