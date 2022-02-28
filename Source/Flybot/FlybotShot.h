// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlybotShot.generated.h"

UCLASS()
class FLYBOT_API AFlybotShot : public AActor
{
	GENERATED_BODY()

public:
	AFlybotShot();

	/** Collision handling function. */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse, const FHitResult& Hit);

	/** Sphere to use for root component and collisions. */
	UPROPERTY(EditAnywhere)
	class USphereComponent* Collision;

	/** Niagara FX component to hold system for flying visual. */
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* FlySystemComponent;

	/** Projectile component to move the actor. */
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* Movement;

	/** Niagara FX system for hit visual. */
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* HitSystem;

	/** How much to change health by when hitting another player. */
	UPROPERTY(EditAnywhere)
	float HealthDelta;

	/** How much to change power by when using this shot. */
	UPROPERTY(EditAnywhere)
	float PowerDelta;
};