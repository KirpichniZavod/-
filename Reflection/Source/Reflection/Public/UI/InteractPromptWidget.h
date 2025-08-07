#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractPromptWidget.generated.h"

class UTextBlock;

UCLASS()
class REFLECTION_API UInteractPromptWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category = "Prompt")
    void SetPromptText(const FText& InText);

protected:
    UPROPERTY(Transient)
    UTextBlock* PromptTextWidget;
};