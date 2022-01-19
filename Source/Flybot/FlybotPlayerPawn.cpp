// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#include "FlybotPlayerPawn.h"
#include "Flybot.h"
#include "FlybotPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

AFlybotPlayerPawn::AFlybotPlayerPawn()
{
	Collision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->SetVisibleFlag(false);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Collision);

	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Body);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Collision);
	SpringArm->SetRelativeLocation(FVector(120.f, 0.f, 50.f));
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 600.f;
	SpringArmLengthScale = 2000.f;
	SpringArmLengthMin = 0.f;
	SpringArmLengthMax = 1000.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(15.f, 0.f, 0.f));
	Camera->PostProcessSettings.bOverride_MotionBlurAmount = true;
	Camera->PostProcessSettings.MotionBlurAmount = 0.1f;

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 5000.f;
	Movement->Acceleration = 5000.f;
	Movement->Deceleration = 10000.f;

	MoveScale = 1.f;
	RotateScale = 50.f;
	bFreeFly = false;
	PrimaryActorTick.bCanEverTick = true;
	ZMovementFrequency = 2.f;
	ZMovementAmplitude = 5.f;
	TiltMax = 15.f;
	TiltMoveScale = 0.6f;
	TiltRotateScale = 0.4f;
	TiltResetScale = 0.3f;
	SpeedCheckInterval = 0.5f;
	MaxMovesWithHits = 30;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void AFlybotPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AFlybotPlayerController* FPC = Cast<AFlybotPlayerController>(Controller);
	check(EIC && FPC);
	EIC->BindAction(FPC->MoveAction, ETriggerEvent::Triggered, this, &AFlybotPlayerPawn::Move);
	EIC->BindAction(FPC->RotateAction, ETriggerEvent::Triggered, this, &AFlybotPlayerPawn::Rotate);
	EIC->BindAction(FPC->FreeFlyAction, ETriggerEvent::Started, this, &AFlybotPlayerPawn::ToggleFreeFly);
	EIC->BindAction(FPC->SpringArmLengthAction, ETriggerEvent::Triggered, this,
		&AFlybotPlayerPawn::UpdateSpringArmLength);

	ULocalPlayer* LocalPlayer = FPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(FPC->PawnMappingContext, 0);
}

void AFlybotPlayerPawn::Move(const FInputActionValue& ActionValue)
{
	FVector Input = ActionValue.Get<FInputActionValue::Axis3D>();
	// UFloatingPawnMovement handles scaling this input based on the DeltaTime for this frame.
	AddMovementInput(GetActorRotation().RotateVector(Input), MoveScale);
	TiltInput += Input.Y * TiltMoveScale * MoveScale;
}

void AFlybotPlayerPawn::Rotate(const FInputActionValue& ActionValue)
{
	FRotator Input(ActionValue[0], ActionValue[1], ActionValue[2]);
	Input *= GetWorld()->GetDeltaSeconds() * RotateScale;
	TiltInput += Input.Yaw * TiltRotateScale;

	if (bFreeFly) {
		AddActorLocalRotation(Input);
	}
	else {
		Input += GetActorRotation();
		Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
		Input.Roll = 0;
		SetActorRotation(Input);
	}
}

void AFlybotPlayerPawn::ToggleFreeFly()
{
	bFreeFly = !bFreeFly;
}

void AFlybotPlayerPawn::UpdateSpringArmLength(const FInputActionValue& ActionValue)
{
	SpringArm->TargetArmLength += ActionValue[0] * GetWorld()->GetDeltaSeconds() * SpringArmLengthScale;
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength,
		SpringArmLengthMin, SpringArmLengthMax);
}

void AFlybotPlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Don't animate or run client RPCs if we're the server.
	if (GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	// Add Z Movement.
	if (ZMovementAmplitude)
	{
		ZMovementTime += DeltaSeconds;
		float ZMovement = FMath::Sin(ZMovementTime * ZMovementFrequency) * ZMovementAmplitude;
		Body->SetRelativeLocation(FVector(0.f, 0.f, ZMovement + ZMovementOffset));
	}

	// Add body and head tilting.
	FRotator Rotation = Body->GetRelativeRotation();

	if (TiltInput != 0.f)
	{
		Rotation.Roll = FMath::Clamp(Rotation.Roll + TiltInput, -TiltMax, TiltMax);
		TiltInput = 0.f;
	}

	// Always try to tilt back towards the center.
	if (Rotation.Roll > 0.f)
	{
		Rotation.Roll -= TiltResetScale;
		if (Rotation.Roll < 0.f)
			Rotation.Roll = 0.f;
	}
	else if (Rotation.Roll < 0.f)
	{
		Rotation.Roll += TiltResetScale;
		if (Rotation.Roll > 0.f)
			Rotation.Roll = 0.f;
	}

	Body->SetRelativeRotation(Rotation);
	Head->SetRelativeRotation(FRotator(0.f, Rotation.Roll, 0.f));

	// Replicate movement to authority if we're controlling the pawn.
	if (Controller && Controller->GetLocalRole() == ROLE_AutonomousProxy)
	{
		UpdateServerTransform(Collision->GetRelativeTransform());
	}
}

void AFlybotPlayerPawn::UpdateServerTransform_Implementation(FTransform Transform)
{
	// Make sure the client does not try to move faster than the game allows. We can't check
	// on each update using the server delta time since the server may tick at different rates
	// than the client, and the server might process multiple updates in one tick. Instead, we
	// calculate an average position every SpeedCheckInterval and check the speed using that.
	float Now = GetWorld()->GetRealTimeSeconds();
	if (SpeedCheckLastTime == 0)
	{
		SpeedCheckLastTranslation = Transform.GetTranslation();
		SpeedCheckLastTime = Now;
		SpeedCheckTranslationSum = FVector::ZeroVector;
		SpeedCheckTranslationCount = 0;
	}
	else
	{
		SpeedCheckTranslationSum += Transform.GetTranslation();
		SpeedCheckTranslationCount++;

		if (Now - SpeedCheckLastTime > SpeedCheckInterval)
		{
			FVector SpeedCheckTranslation = SpeedCheckTranslationSum / SpeedCheckTranslationCount;
			float Distance = FVector::Distance(SpeedCheckLastTranslation, SpeedCheckTranslation);
			float Speed = Distance / (Now - SpeedCheckLastTime);
			//UE_LOG(LogFlybot, Log, TEXT("Client speed update: %s %.3f %d"), *Controller->GetName(), Speed, SpeedCheckCount);

			SpeedCheckLastTime = Now;
			SpeedCheckTranslationSum = FVector::ZeroVector;
			SpeedCheckTranslationCount = 0;

			// Allow 5% more than MaxSpeed to account for time and translation variation.
			if (Speed > Movement->MaxSpeed * 1.05f)
			{
				// Moving too fast, ignore update and move client back to last translation.
				UE_LOG(LogFlybot, Log, TEXT("Player moving too fast: %s %.3f"), *Controller->GetName(), Speed);
				UpdateClientTransform(FTransform(Collision->GetRelativeRotation(), SpeedCheckLastTranslation));
				return;
			}

			SpeedCheckLastTranslation = SpeedCheckTranslation;
		}
	}

	// Move client with a sweep to see if we hit anything. We seem to get hits on the server even when
	// the client sends valid moves, especially while sliding against objects. We'll always have a valid
	// move on the server since the sweep will correct the server side. We expect the client to eventually
	// send a transform that moves cleanly, but if we go too long (MaxMovesWithHits), send a correction
	// back to the client. This will cause a stutter on the client so we want to keep it minimal.
	FTransform OldTransform = Collision->GetRelativeTransform();
	FHitResult HitResult;
	Collision->SetRelativeTransform(Transform, true, &HitResult);
	if (HitResult.bBlockingHit) {
		//float ExpectedDistance = FVector::Distance(HitResult.TraceStart, HitResult.TraceEnd);
		//UE_LOG(LogFlybot, Log, TEXT("Player hit object: %s %d (%.3f-%.3f=%.3f)"), *Controller->GetName(),
		//	MovesWithHits, ExpectedDistance, HitResult.Distance, ExpectedDistance - HitResult.Distance);
		MovesWithHits++;
	}
	else {
		//UE_LOG(LogFlybot, Log, TEXT("Player move ok: %s"), *Controller->GetName());
		MovesWithHits = 0;
	}

	if (MovesWithHits > MaxMovesWithHits) {
		UE_LOG(LogFlybot, Log, TEXT("Correcting player transform: %s"), *Controller->GetName());
		UpdateClientTransform(Collision->GetRelativeTransform());
	}
}

void AFlybotPlayerPawn::UpdateClientTransform_Implementation(FTransform Transform)
{
	Collision->SetRelativeTransform(Transform);
}