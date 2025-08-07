#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseMenuResumeRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseMenuQuitRequested);

UCLASS()
class REFLECTION_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "PauseMenu")
    FOnPauseMenuResumeRequested OnResumeRequested;

    UPROPERTY(BlueprintAssignable, Category = "PauseMenu")
    FOnPauseMenuQuitRequested OnQuitRequested;

    UFUNCTION(BlueprintCallable, Category = "PauseMenu")
    void RequestResume() { OnResumeRequested.Broadcast(); }

    UFUNCTION(BlueprintCallable, Category = "PauseMenu")
    void RequestQuit() { OnQuitRequested.Broadcast(); }
};