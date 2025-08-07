#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ReflectionCharacter.generated.h"

UCLASS()
class REFLECTION_API AReflectionCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AReflectionCharacter();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UCameraComponent* FirstPersonCamera;

    UPROPERTY(EditDefaultsOnly, Category = "Interaction")
    float InteractTraceDistance;

    UFUNCTION()
    void MoveForward(float Value);

    UFUNCTION()
    void MoveRight(float Value);

    UFUNCTION()
    void Turn(float Value);

    UFUNCTION()
    void LookUp(float Value);

    UFUNCTION()
    void Interact();

    UFUNCTION()
    void ToggleMenu();

    void PerformInteractTrace();
    void TryInteractWith(AActor* HitActor);

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};