// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#include "FlybotMapRoom.h"
#include "Flybot.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"

AFlybotMapRoom::AFlybotMapRoom()
{
	PrimaryActorTick.bCanEverTick = false;
	GridSize = 1000.f;
	RoomSize = 3;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	Walls = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Walls"));
	Walls->SetupAttachment(SceneComponent);

	Edges = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Edges"));
	Edges->SetupAttachment(SceneComponent);

	Corners = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Corners"));
	Corners->SetupAttachment(SceneComponent);

	TubeWalls = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TubeWalls"));
	TubeWalls->SetupAttachment(SceneComponent);

	Tubes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tubes"));
	Tubes->SetupAttachment(SceneComponent);
}

/** Helper function to keep calls in OnConstruction concise. */
static FORCEINLINE void AddInstance(UInstancedStaticMeshComponent* Component,
	const FRotator& Rotation, const FVector& Translation)
{
	Component->AddInstance(FTransform(Rotation, Rotation.RotateVector(Translation)));
}

/**
 * Rotations used while adding mesh instances. These assume the mesh
 * has been created with a base orientation of positive X.
 */
static const FRotator PositiveX(0.f, 0.f, 0.f);
static const FRotator PositiveX90(0.f, 0.f, 90.f);
static const FRotator PositiveX180(0.f, 0.f, 180.f);
static const FRotator PositiveX270(0.f, 0.f, 270.f);
static const FRotator NegativeX(0.f, 180.f, 0.f);
static const FRotator NegativeX90(0.f, 180.f, 90.f);
static const FRotator NegativeX180(0.f, 180.f, 180.f);
static const FRotator NegativeX270(0.f, 180.f, 270.f);

static const FRotator PositiveY(0.f, 90.f, 0.f);
static const FRotator PositiveY180(0.f, 90.f, 180.f);
static const FRotator NegativeY(0.f, 270.f, 0.f);
static const FRotator NegativeY180(0.f, 270.f, 180.f);

static const FRotator PositiveZ(90.f, 0.f, 0.f);
static const FRotator NegativeZ(-90.f, 0.f, 0.f);

void AFlybotMapRoom::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Only rebuild if needed.
	if (BuiltGridSize == GridSize &&
		BuiltRoomSize == RoomSize &&
		BuiltPositiveXTubeSize == PositiveXTubeSize &&
		BuiltNegativeXTubeSize == NegativeXTubeSize &&
		BuiltPositiveYTubeSize == PositiveYTubeSize &&
		BuiltNegativeYTubeSize == NegativeYTubeSize &&
		BuiltPositiveZTubeSize == PositiveZTubeSize &&
		BuiltNegativeZTubeSize == NegativeZTubeSize)
	{
		return;
	}

	UE_LOG(LogFlybot, Log,
		TEXT("AFlybotMapRoom::OnConstruction Building Room Size %d (Built=%d this=%x)"),
		RoomSize, BuiltRoomSize, this);

	BuiltGridSize = GridSize;
	BuiltRoomSize = RoomSize;
	BuiltPositiveXTubeSize = PositiveXTubeSize;
	BuiltNegativeXTubeSize = NegativeXTubeSize;
	BuiltPositiveYTubeSize = PositiveYTubeSize;
	BuiltNegativeYTubeSize = NegativeYTubeSize;
	BuiltPositiveZTubeSize = PositiveZTubeSize;
	BuiltNegativeZTubeSize = NegativeZTubeSize;

	Walls->ClearInstances();
	Edges->ClearInstances();
	Corners->ClearInstances();
	TubeWalls->ClearInstances();
	Tubes->ClearInstances();

	// Implicit floor with integer division, which makes all room sizes end up being odd.
	int32 HalfSize = RoomSize / 2;
	int32 WallOffset = (HalfSize + 1) * GridSize;
	FVector Translation(WallOffset, 0.f, 0.f);

	for (int32 a = -HalfSize; a <= HalfSize; a++)
	{
		Translation.Y = GridSize * a;

		for (int32 b = -HalfSize; b <= HalfSize; b++)
		{
			Translation.Z = GridSize * b;

			// Build walls, placing a tube wall in the center if needed.
			auto WallType = [&](uint32 TubeSize)
			{
				return (a == 0 && b == 0 && TubeSize > 0) ? TubeWalls : Walls;
			};

			AddInstance(WallType(PositiveXTubeSize), PositiveX, Translation);
			AddInstance(WallType(NegativeXTubeSize), NegativeX, Translation);
			AddInstance(WallType(PositiveYTubeSize), PositiveY, Translation);
			AddInstance(WallType(NegativeYTubeSize), NegativeY, Translation);
			AddInstance(WallType(PositiveZTubeSize), PositiveZ, Translation);
			AddInstance(WallType(NegativeZTubeSize), NegativeZ, Translation);
		}

		// Build edges.
		Translation.Z = WallOffset;
		AddInstance(Edges, PositiveX, Translation);
		AddInstance(Edges, PositiveX90, Translation);
		AddInstance(Edges, PositiveX180, Translation);
		AddInstance(Edges, PositiveX270, Translation);
		AddInstance(Edges, NegativeX, Translation);
		AddInstance(Edges, NegativeX90, Translation);
		AddInstance(Edges, NegativeX180, Translation);
		AddInstance(Edges, NegativeX270, Translation);
		AddInstance(Edges, PositiveY, Translation);
		AddInstance(Edges, PositiveY180, Translation);
		AddInstance(Edges, NegativeY, Translation);
		AddInstance(Edges, NegativeY180, Translation);
	}

	// Build corners.
	Translation.Y = WallOffset;
	AddInstance(Corners, PositiveX, Translation);
	AddInstance(Corners, PositiveX90, Translation);
	AddInstance(Corners, PositiveX180, Translation);
	AddInstance(Corners, PositiveX270, Translation);
	AddInstance(Corners, NegativeX, Translation);
	AddInstance(Corners, NegativeX90, Translation);
	AddInstance(Corners, NegativeX180, Translation);
	AddInstance(Corners, NegativeX270, Translation);

	// Build tubes.
	Translation.Y = 0.f;
	Translation.Z = 0.f;
	auto AddTubeInstances = [&](uint32 TubeSize, const FRotator& Rotation)
	{
		// Start at 1 because the first tube is the tube wall added above.
		for (uint32 a = 1; a < TubeSize; a++)
		{
			Translation.X = WallOffset + GridSize * a;
			AddInstance(Tubes, Rotation, Translation);
		}
	};

	AddTubeInstances(PositiveXTubeSize, PositiveX);
	AddTubeInstances(NegativeXTubeSize, NegativeX);
	AddTubeInstances(PositiveYTubeSize, PositiveY);
	AddTubeInstances(NegativeYTubeSize, NegativeY);
	AddTubeInstances(PositiveZTubeSize, PositiveZ);
	AddTubeInstances(NegativeZTubeSize, NegativeZ);
}