#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseMenuResumeRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseMenuQuitRequested);

UCLASS()
class REFLECTION_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintAssignable, Category = "PauseMenu")
    FOnPauseMenuResumeRequested OnResumeRequested;

    UPROPERTY(BlueprintAssignable, Category = "PauseMenu")
    FOnPauseMenuQuitRequested OnQuitRequested;

protected:
    UPROPERTY(Transient)
    UVerticalBox* RootBox;

    UPROPERTY(Transient)
    UButton* ResumeButton;

    UPROPERTY(Transient)
    UButton* QuitButton;

    UFUNCTION()
    void OnResumeClicked();

    UFUNCTION()
    void OnQuitClicked();
};