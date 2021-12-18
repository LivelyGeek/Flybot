// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#include "FlybotGameMode.h"
#include "Flybot.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

AFlybotGameMode::AFlybotGameMode()
{
}

void AFlybotGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    UE_LOG(LogFlybot, Log, TEXT("Game is running: %s %s"), *MapName, *Options);

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		FreePlayerStarts.Add(*It);
		UE_LOG(LogFlybot, Log, TEXT("Found player start: %s"), *(*It)->GetName());
	}
}

void AFlybotGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (FreePlayerStarts.Num() == 0)
	{
		ErrorMessage = TEXT("Server full");
	}

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

FString AFlybotGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	if (FreePlayerStarts.Num() == 0)
	{
		UE_LOG(LogFlybot, Log, TEXT("No free player starts in InitNewPlayer"));
		return FString(TEXT("No free player starts"));
	}

	NewPlayerController->StartSpot = FreePlayerStarts.Pop();
	UE_LOG(LogFlybot, Log, TEXT("Using player start %s for %s"),
		*NewPlayerController->StartSpot->GetName(), *NewPlayerController->GetName());
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}