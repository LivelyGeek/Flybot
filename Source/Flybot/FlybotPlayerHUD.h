// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FlybotPlayerHUD.generated.h"

UCLASS(Abstract)
class FLYBOT_API UFlybotPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Update HUD with current health. */
	void SetHealth(float CurrentHealth, float MaxHealth);

	/** Update HUD with current power. */
	void SetPower(float CurrentPower, float MaxPower);

	/** Widget to use to display current health. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	/** Widget to use to display current power. */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* PowerBar;
};