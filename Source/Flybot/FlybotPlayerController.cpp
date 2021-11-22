// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#include "FlybotPlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"

/** Map key to action for mapping context with optional modifiers. */
static void MapKey(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
	bool bNegate = false,
	bool bSwizzle = false, EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
{
	FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(InputAction, Key);
	UObject* Outer = InputMappingContext->GetOuter();

	if (bNegate)
	{
		UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(Outer);
		Mapping.Modifiers.Add(Negate);
	}

	if (bSwizzle)
	{
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
		Swizzle->Order = SwizzleOrder;
		Mapping.Modifiers.Add(Swizzle);
	}
}

void AFlybotPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Create these objects here and not in constructor because we only need them on the client.
	PawnMappingContext = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis3D;
	MapKey(PawnMappingContext, MoveAction, EKeys::W);
	MapKey(PawnMappingContext, MoveAction, EKeys::S, true);
	MapKey(PawnMappingContext, MoveAction, EKeys::A, true, true);
	MapKey(PawnMappingContext, MoveAction, EKeys::D, false, true);
	MapKey(PawnMappingContext, MoveAction, EKeys::SpaceBar, false, true, EInputAxisSwizzle::ZYX);
	MapKey(PawnMappingContext, MoveAction, EKeys::LeftShift, true, true, EInputAxisSwizzle::ZYX);

	RotateAction = NewObject<UInputAction>(this);
	RotateAction->ValueType = EInputActionValueType::Axis3D;
	MapKey(PawnMappingContext, RotateAction, EKeys::MouseY);
	MapKey(PawnMappingContext, RotateAction, EKeys::MouseX, false, true);
	MapKey(PawnMappingContext, RotateAction, EKeys::Q, true, true, EInputAxisSwizzle::ZYX);
	MapKey(PawnMappingContext, RotateAction, EKeys::E, false, true, EInputAxisSwizzle::ZYX);

	FreeFlyAction = NewObject<UInputAction>(this);
	MapKey(PawnMappingContext, FreeFlyAction, EKeys::F);

	SpringArmLengthAction = NewObject<UInputAction>(this);
	SpringArmLengthAction->ValueType = EInputActionValueType::Axis1D;
	MapKey(PawnMappingContext, SpringArmLengthAction, EKeys::MouseWheelAxis);
}