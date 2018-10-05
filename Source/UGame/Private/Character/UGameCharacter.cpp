// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#include "UGameCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"

#include "../Private/Character/Components/GMovement.h"
#include "../Private/Character/Components/GPickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////

// Sets default values
AUGameCharacter::AUGameCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(30.f, 96.0f);

    // Create a CameraComponent	
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->RelativeLocation = FVector(-5.56f, 1.75f, 64.f); // Position the camera
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
    HoldingComponent->RelativeLocation.X = 50.0f;

    CurrentItem = NULL;
    bCanMove = true;

    // Setup for the components
    MovementComponent = CreateDefaultSubobject<UGMovement>(TEXT("Movement Component"));

    PickupComponent = CreateDefaultSubobject<UGPickup>(TEXT("Pickup Component"));
    PickupComponent->PlayerCharacter = this;
}

void AUGameCharacter::BeginPlay()
{
    // Call the base class  
    Super::BeginPlay();

}

//Called every frame
void AUGameCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Start = FirstPersonCameraComponent->GetComponentLocation();
    ForwardVector = FirstPersonCameraComponent->GetForwardVector();
    End = ((ForwardVector * 200.f) + Start);

    if (!bHoldingItem)
    {
        if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam))
        {
            if (Hit.GetActor()->GetClass()->IsChildOf(APickupAndRotateActor::StaticClass()))
            {
                CurrentItem = Cast<APickupAndRotateActor>(Hit.GetActor());
            }
        }
        else
        {
            CurrentItem = NULL;
        }
    }

    FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(FirstPersonCameraComponent->FieldOfView, 90.0f, 0.1f));

    if (bHoldingItem)
    {
        HoldingComponent->SetRelativeLocation(FVector(50.0f, 0.0f, 0.f));
    }
    
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (MovementComponent != nullptr) MovementComponent->SetupKeyBindings(PlayerInputComponent);
    if (PickupComponent != nullptr) PickupComponent->SetupKeyBindings(PlayerInputComponent);

    // Default Camera view bindings
    PlayerInputComponent->BindAxis("CameraPitch", this, &AUGameCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("CameraYaw", this, &AUGameCharacter::AddControllerYawInput);
}

void AUGameCharacter::AddControllerPitchInput(const float Val) {
    if (bCanMove == false) return;

    this->AddControllerPitchInput(Val);
}

void AUGameCharacter::AddControllerYawInput(const float Val) {
    if (bCanMove == false) return;

    this->AddControllerYawInput(Val);

}

void AUGameCharacter::OnAction()
{
    if (CurrentItem)
    {
        ToggleItemPickup();
    }
}

void AUGameCharacter::ToggleMovement()
{
    bCanMove = !bCanMove;
    FirstPersonCameraComponent->bUsePawnControlRotation = !FirstPersonCameraComponent->bUsePawnControlRotation;
    bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void AUGameCharacter::ToggleItemPickup()
{
    if (CurrentItem)
    {
        bHoldingItem = !bHoldingItem;
        CurrentItem->Pickup();

        if (!bHoldingItem)
        {
            CurrentItem = NULL;
        }
    }
}

