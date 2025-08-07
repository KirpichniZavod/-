#include "ReflectionCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"
#include "ReflectionPlayerController.h"

AReflectionCharacter::AReflectionCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
    FirstPersonCamera->bUsePawnControlRotation = true;

    bUseControllerRotationYaw = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = 300.f;

    InteractTraceDistance = 250.f;
}

void AReflectionCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AReflectionCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    PerformInteractTrace(true);
}

void AReflectionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AReflectionCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AReflectionCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AReflectionCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AReflectionCharacter::LookUp);

    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AReflectionCharacter::Interact);
    PlayerInputComponent->BindAction(TEXT("Menu"), IE_Pressed, this, &AReflectionCharacter::ToggleMenu);
}

void AReflectionCharacter::MoveForward(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;
    AddMovementInput(GetActorForwardVector(), Value);
}

void AReflectionCharacter::MoveRight(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;
    AddMovementInput(GetActorRightVector(), Value);
}

void AReflectionCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void AReflectionCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AReflectionCharacter::Interact()
{
    PerformInteractTrace(false);
}

void AReflectionCharacter::PerformInteractTrace(bool bForHover)
{
    const FVector Start = FirstPersonCamera->GetComponentLocation();
    const FVector End = Start + FirstPersonCamera->GetForwardVector() * InteractTraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractTrace), false, this);
    const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

#if !(UE_BUILD_SHIPPING)
    DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Yellow : FColor::Green, false, 0.0f, 0, 0.25f);
#endif

    AActor* NewHovered = bHit ? Hit.GetActor() : nullptr;

    if (bForHover)
    {
        if (HoveredActor.Get() != NewHovered)
        {
            HoveredActor = NewHovered;
            AReflectionPlayerController* PC = Cast<AReflectionPlayerController>(GetController());
            if (PC)
            {
                if (NewHovered && NewHovered->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
                {
                    const FText Name = IInteractable::Execute_GetDisplayName(NewHovered);
                    PC->ShowInteractPrompt(Name.IsEmpty() ? FText::FromString(TEXT("Нажмите E, чтобы взаимодействовать")) : Name);
                }
                else
                {
                    PC->HideInteractPrompt();
                }
            }
        }
        return;
    }

    if (bHit && NewHovered)
    {
        TryInteractWith(NewHovered);
    }
}

void AReflectionCharacter::TryInteractWith(AActor* HitActor)
{
    if (!HitActor) return;

    if (HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        IInteractable::Execute_Interact(HitActor, this);
    }
}

void AReflectionCharacter::ToggleMenu()
{
    if (AReflectionPlayerController* PC = Cast<AReflectionPlayerController>(GetController()))
    {
        PC->TogglePauseMenu();
    }
}