// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Loader/ModLoader/TrackLoader.h"
#include "Loader/ModLoader/ChassiLoader.h"
#include "Loader/ModLoader/TireLoader.h"
#include "MainGameInstance.generated.h"

/**
 *
 */
UCLASS()
class KARTWORLD_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	IPlatformFile &FileManager = FPlatformFileManager::Get().GetPlatformFile();

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString GameUserDir;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UTrackLoader* TrackLoader = NewObject<UTrackLoader>();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UChassiLoader* ChassiLoader = NewObject<UChassiLoader>();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UTireLoader* TireLoader = NewObject<UTireLoader>();

	virtual void Init() override;
};
