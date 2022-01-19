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

	/** Handle input to toggle free flying. */
	void ToggleFreeFly();

	/** Handle input to update spring arm length. */
	void UpdateSpringArmLength(const struct FInputActionValue& ActionValue);

	/** Update pawn and components every frame. */
	void Tick(float DeltaSeconds) override;

	/** Update server with latest transform from the client. */
	UFUNCTION(Server, Unreliable)
	void UpdateServerTransform(FTransform Transform);

	/** Update client transform when server needs to send a correction. */
	UFUNCTION(Client, Unreliable)
	void UpdateClientTransform(FTransform Transform);

	/** Static mesh to use for root component and collisions. */
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Collision;

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

	/** Scale to apply to spring arm length input. */
	UPROPERTY(EditAnywhere)
	float SpringArmLengthScale;

	/** Minimum spring arm length. */
	UPROPERTY(EditAnywhere)
	float SpringArmLengthMin;

	/** Maximum spring arm length. */
	UPROPERTY(EditAnywhere)
	float SpringArmLengthMax;

	/** Amplitude to scale Z movement by, 0 to disable Z movement. */
	UPROPERTY(EditAnywhere)
	float ZMovementAmplitude;

	/** Frequency to adjust Z movement by. */
	UPROPERTY(EditAnywhere)
	float ZMovementFrequency;

	/** Offset to apply to Z movement after after calculating amplitude. */
	UPROPERTY(EditAnywhere)
	float ZMovementOffset;

	/** Max tilt to apply to body and head while turning. */
	UPROPERTY(EditAnywhere)
	float TiltMax;

	/** Scale to apply to movement input for tilt. */
	UPROPERTY(EditAnywhere)
	float TiltMoveScale;

	/** Scale to apply to rotation input for tilt. */
	UPROPERTY(EditAnywhere)
	float TiltRotateScale;

	/** Scale to apply to tilt when resetting to center. */
	UPROPERTY(EditAnywhere)
	float TiltResetScale;

	/** How often to check speed with average translation for each interval. */
	UPROPERTY(EditAnywhere)
	float SpeedCheckInterval;

	/** Max number of consecutive moves with hits to allow from client. */
	UPROPERTY(EditAnywhere)
	uint32 MaxMovesWithHits;

private:
	/** Time to calculate Z movement from. */
	float ZMovementTime;

	/** The current input to apply to tilt. */
	float TiltInput;

	/** Current sum of translations used for average in next speed check. */
	FVector SpeedCheckTranslationSum;

	/** Current count of translations used for average in next speed check. */
	uint32 SpeedCheckTranslationCount;

	/** Last average translation used in speed check. */
	FVector SpeedCheckLastTranslation;

	/** Last time speed was checked. */
	float SpeedCheckLastTime;

	/** How many consecutive moves with hits we've seen from client. */
	uint32 MovesWithHits;
};