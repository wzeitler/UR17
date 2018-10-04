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

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

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

    PitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
    PitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;

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

    /*
    // set up gameplay key bindings
    check(PlayerInputComponent);

    // Bind action event
    PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AUGameCharacter::OnAction);

    // Bind movement events
    PlayerInputComponent->BindAxis("MoveForward", this, &AUGameCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AUGameCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AUGameCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AUGameCharacter::LookUpAtRate);
    */
}

void AUGameCharacter::MoveForward(float Value)
{
    if (Value != 0.0f && bCanMove)
    {
        // add movement in that direction
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void AUGameCharacter::MoveRight(float Value)
{
    if (Value != 0.0f && bCanMove)
    {
        // add movement in that direction
        AddMovementInput(GetActorRightVector(), Value);
    }
}

void AUGameCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUGameCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
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

