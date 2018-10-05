// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/PickupAndRotateActor.h"
#include "UGameCharacter.generated.h"

UCLASS()
class UGAME_API AUGameCharacter : public ACharacter
{
    GENERATED_BODY()

    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FirstPersonCameraComponent;

    /** Holding Component */
    UPROPERTY(EditAnywhere)
    class USceneComponent* HoldingComponent;

public:
    AUGameCharacter();

    // Components attached to this actor
    UPROPERTY(EditAnywhere, Instanced)
        UGMovement* MovementComponent;

    UPROPERTY(EditAnywhere, Instanced)
        UGPickup* PickupComponent;

protected:
    virtual void BeginPlay();

    virtual void Tick(float DeltaSeconds) override;

public:
    void AddControllerPitchInput(const float Val);

    void AddControllerYawInput(const float Val);
   
    UPROPERTY(EditAnywhere)
        class APickupAndRotateActor* CurrentItem;

    bool bCanMove;
    bool bHoldingItem;

    FVector HoldingComp;
    FRotator LastRotation;

    FVector Start;
    FVector ForwardVector;
    FVector End;

    FHitResult Hit;

    FComponentQueryParams DefaultComponentQueryParams;
    FCollisionResponseParams DefaultResponseParam;

protected:

    /** Action Function */
    void OnAction();

    // toggle player movement
    void ToggleMovement();

    // toggle holding item pickup
    void ToggleItemPickup();

protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    /** Returns FirstPersonCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
