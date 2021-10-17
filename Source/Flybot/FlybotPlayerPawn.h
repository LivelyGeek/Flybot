// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlybotPlayerPawn.generated.h"

UCLASS()
class FLYBOT_API AFlybotPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AFlybotPlayerPawn();

	/** Bind input actions from player controller. */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Handle input to update location. */
	void Move(const struct FInputActionValue& ActionValue);

	/** Handle input to update rotation. */
	void Rotate(const struct FInputActionValue& ActionValue);

	/** Handle input to toggle free flying */
	void ToggleFreeFly();

	/** Sphere to use for root component and collisions. */
	UPROPERTY(EditAnywhere)
	class USphereComponent* Sphere;

	/** Static mesh for the body, attached to the root. */
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Body;

	/** Static mesh for the head, attached to the body. */
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Head;

	/** Spring arm to hold camera, attached to the root. */
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	/** Camera attached to spring arm to provide pawn's view. */
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	/** Use floating pawn movement to smooth out motion. */
	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* Movement;

	/** Scale to apply to location input. */
	UPROPERTY(EditAnywhere)
	float MoveScale;

	/** Scale to apply to rotation input. */
	UPROPERTY(EditAnywhere)
	float RotateScale;

	/** Whether to use free flying mode. Caution: might cause motion sickness! */
	UPROPERTY(EditAnywhere)
	uint32 bFreeFly:1;
};