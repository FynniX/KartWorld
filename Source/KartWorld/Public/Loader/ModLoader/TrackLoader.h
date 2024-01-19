// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "Loader/ModLoader/ModLoader.h"
#include "Loader/MeshLoader.h"
#include "IniLibrary.h"
#include "TrackLoader.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTrackLoader, Log, All);

USTRUCT(BlueprintType)
struct KARTWORLD_API FTrackConfiguration {
	GENERATED_BODY()

public:
	// Details Section
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Details")
	FString Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Details")
	FString Description;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Details")
	int32 Length = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Details")
	int32 Altitude = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Details")
	FString Author;

	// Location Section
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Location")
	float Latitude = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Location")
	float Longitude = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Location")
	int32 Timezone = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Location")
	bool bDst = false;

	// Gfx Section
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gfx")
	FString Model;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FTrackModel {
	GENERATED_BODY()

public:
	FTrackModel();
	FTrackModel(
		FTrackConfiguration Configuration,
		FModelData Model
	);

	// Success
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bSuccess = false;

	// Configuration file
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FTrackConfiguration Configuration;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FModelData Model;
};

/**
 * 
 */
UCLASS()
class KARTWORLD_API UTrackLoader : public UModLoader
{
	GENERATED_BODY()

private:
	// Getting the FileManager
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	bool GetConfiguration(FString FilePath, FTrackConfiguration& TrackConfiguration);
	
public:
	// Mods Directory
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString ModsDir;

	UTrackLoader();

	// Initialize loader in game instance
	virtual void Initialize(FString GameUserDir) override;

	// Load track mod by name
	UFUNCTION(BlueprintCallable)
	FTrackModel Load(FString TrackName);
};
