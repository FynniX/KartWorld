// Copyright @ 2023 Fynn Haupt

#include "Loader/ModLoader/ModLoader.h"

UModLoader::UModLoader()
{
}

void UModLoader::Initialize(FString GameUserDir)
{
	// Getting the FileManager
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	// Gets game mod directory (eg. Documents/KartSimWorld/mods), if doesn't exists then create one.
	GameModDir = FPaths::Combine(GameUserDir, "mods");
	if (!FileManager.DirectoryExists(*GameModDir)) FileManager.CreateDirectory(*GameModDir);
}