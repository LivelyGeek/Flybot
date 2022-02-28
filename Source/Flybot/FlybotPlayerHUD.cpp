// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#include "FlybotPlayerHUD.h"
#include "Components/ProgressBar.h"

void UFlybotPlayerHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UFlybotPlayerHUD::SetPower(float CurrentPower, float MaxPower)
{
	if (PowerBar)
	{
		PowerBar->SetPercent(CurrentPower / MaxPower);
	}
}