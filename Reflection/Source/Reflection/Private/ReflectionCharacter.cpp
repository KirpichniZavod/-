#include "ReflectionCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"

AReflectionCharacter::AReflectionCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
    FirstPersonCamera->bUsePawnControlRotation = true;

    bUseControllerRotationYaw = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->MaxWalkSpeed = 300.f; // базовая ходьба

    InteractTraceDistance = 250.f;
}

void AReflectionCharacter::BeginPlay()
{
    Super::BeginPlay();
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
    PerformInteractTrace();
}

void AReflectionCharacter::PerformInteractTrace()
{
    const FVector Start = FirstPersonCamera->GetComponentLocation();
    const FVector End = Start + FirstPersonCamera->GetForwardVector() * InteractTraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractTrace), false, this);
    const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

#if !(UE_BUILD_SHIPPING)
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 0.5f);
#endif

    if (bHit && Hit.GetActor())
    {
        TryInteractWith(Hit.GetActor());
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
    const bool bNewPaused = !UGameplayStatics::IsGamePaused(this);
    UGameplayStatics::SetGamePaused(this, bNewPaused);
}