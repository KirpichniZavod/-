#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChoiceEntryWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceSelected, int32, Index);

UCLASS()
class REFLECTION_API UChoiceEntryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintAssignable, Category = "Choice")
    FOnChoiceSelected OnSelected;

    UFUNCTION(BlueprintCallable, Category = "Choice")
    void SetIndex(int32 InIndex) { Index = InIndex; }

    UFUNCTION(BlueprintCallable, Category = "Choice")
    void SetText(const FText& InText);

protected:
    UPROPERTY(Transient)
    UButton* Button;

    UPROPERTY(Transient)
    UTextBlock* Label;

    int32 Index = INDEX_NONE;

    UFUNCTION()
    void HandleClicked();
};