// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "FlybotGameMode.h"
#include "Flybot.h"

AFlybotGameMode::AFlybotGameMode()
{
}

void AFlybotGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    UE_LOG(LogFlybot, Log, TEXT("Game is running: %s %s"), *MapName, *Options);
}