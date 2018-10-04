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

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        float BaseLookUpRate;
   
    UPROPERTY(EditAnywhere)
        class APickupAndRotateActor* CurrentItem;

    bool bCanMove;
    bool bHoldingItem;

    float PitchMax;
    float PitchMin;

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

    /** Handles moving forward/backward */
    void MoveForward(float Val);

    /** Handles stafing movement, left and right */
    void MoveRight(float Val);

    /**
     * Called via input to turn at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);

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
