// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlybotGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FLYBOT_API AFlybotGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFlybotGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
};
