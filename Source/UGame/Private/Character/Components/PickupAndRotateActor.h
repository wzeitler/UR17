// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Waldemar Zeitler

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "PickupAndRotateActor.generated.h"

UCLASS()
class UGAME_API APickupAndRotateActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    APickupAndRotateActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* MyMesh;

    UPROPERTY(EditAnywhere)
        USceneComponent* HoldingComp;

    UFUNCTION()
        void RotateActor();

    UFUNCTION()
        void Pickup();

    bool bHolding;
    bool bGravity;

    FRotator ControlRotation;
    ACharacter* MyCharacter;
    UCameraComponent* PlayerCamera;
    FVector ForwardVector;

};
