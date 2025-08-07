#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueTypes.generated.h"

USTRUCT(BlueprintType)
struct FDialogueChoice
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Text;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NextLineIndex = INDEX_NONE; // -1 завершить диалог
};

USTRUCT(BlueprintType)
struct FDialogueLine
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Speaker;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Text;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDialogueChoice> Choices;
};

UCLASS(BlueprintType)
class REFLECTION_API UDialogueDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FDialogueLine> Lines;
};