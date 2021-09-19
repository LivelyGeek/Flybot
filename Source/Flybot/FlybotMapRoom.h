// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlybotMapRoom.generated.h"

UCLASS()
class FLYBOT_API AFlybotMapRoom : public AActor
{
	GENERATED_BODY()

public:
	AFlybotMapRoom();

	/** Build or rebuild the room if needed. */
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Static mesh to use for walls. */
	UPROPERTY(EditAnywhere)
	class UInstancedStaticMeshComponent* Walls;

	/** Static mesh to use for edges where two walls meet. */
	UPROPERTY(EditAnywhere)
	class UInstancedStaticMeshComponent* Edges;

	/** Static mesh to use for corners where three edges meet. */
	UPROPERTY(EditAnywhere)
	class UInstancedStaticMeshComponent* Corners;

	/** Static mesh to use for walls where tubes exit room. */
	UPROPERTY(EditAnywhere)
	class UInstancedStaticMeshComponent* TubeWalls;

	/** Static mesh to use for tubes extending from tube walls. */
	UPROPERTY(EditAnywhere)
	class UInstancedStaticMeshComponent* Tubes;

	/** Size of grid to use when placing meshes. */
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	float GridSize;

	/** Size of the room to build. This will round up to the next odd number. */
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, ClampMax = 25))
	uint32 RoomSize;

	/** How many tubes to extend off the center positive X wall, if any (0 to disable). */
	UPROPERTY(EditAnywhere, meta = (ClampMax = 1000))
	uint32 PositiveXTubeSize;

	/** How many tubes to extend off the center negative X wall, if any (0 to disable). */
	UPROPERTY(EditAnywhere, meta = (ClampMax = 1000))
	uint32 NegativeXTubeSize;

	/** How many tubes to extend off the center positive Y wall, if any (0 to disable). */
	UPROPERTY(EditAnywhere, meta = (ClampMax = 1000))
	uint32 PositiveYTubeSize;

	/** How many tubes to extend off the center negative Y wall, if any (0 to disable). */
	UPROPERTY(EditAnywhere, meta = (ClampMax = 1000))
	uint32 NegativeYTubeSize;

	/** How many tubes to extend off the center positive Z wall, if any (0 to disable). */
	UPROPERTY(EditAnywhere, meta = (ClampMax = 1000))
	uint32 PositiveZTubeSize;

	/** How many tubes to extend off the center negative Z wall, if any (0 to disable). */
	UPROPERTY(EditAnywhere, meta = (ClampMax = 1000))
	uint32 NegativeZTubeSize;

private:
	/* Variables saved when building to know if we need to rebuild when OnConstruction is called. */
	uint32 BuiltGridSize;
	uint32 BuiltRoomSize;
	uint32 BuiltPositiveXTubeSize;
	uint32 BuiltNegativeXTubeSize;
	uint32 BuiltPositiveYTubeSize;
	uint32 BuiltNegativeYTubeSize;
	uint32 BuiltPositiveZTubeSize;
	uint32 BuiltNegativeZTubeSize;

	void AddPointLight(float Intensity, float Radius,
		const FRotator& Rotation, const FVector& Translation);
};