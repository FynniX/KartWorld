// Copyright @ 2023 Fynn Haupt


#include "MainGameInstance.h"

void UMainGameInstance::Init()
{
    Super::Init();

    // Gets game user directory (eg. Documents/KartSimWorld), if doesn't exists then create one.
	GameUserDir = FString(FPlatformProcess::UserDir()).Append("KartWorld/");
	if (!FileManager.DirectoryExists(*GameUserDir)) FileManager.CreateDirectory(*GameUserDir);

    // Init mod loader
    TrackLoader->Initialize(GameUserDir);
    ChassiLoader->Initialize(GameUserDir);
    TireLoader->Initialize(GameUserDir);
}
