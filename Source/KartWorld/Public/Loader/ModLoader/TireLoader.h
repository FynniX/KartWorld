// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "Loader/ModLoader/ModLoader.h"
#include "Loader/MeshLoader.h"
#include "IniLibrary.h"
#include "TireLoader.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTireLoader, Log, All);

USTRUCT(BlueprintType)
struct KARTWORLD_API FTireConfiguration
{
	GENERATED_BODY()

public:
	// Details Section
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Details")
	FString Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Details")
	FString Description;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Details")
	FString Author;

	// Gfx Section
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gfx")
	FString ModelFl;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gfx")
	FString ModelFr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gfx")
	FString ModelRl;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gfx")
	FString ModelRr;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FTireModel
{
	GENERATED_BODY()

public:
	FTireModel();
	FTireModel(
		FTireConfiguration Configuration,
		FModelData ModelFl,
		FModelData ModelFr,
		FModelData ModelRl,
		FModelData ModelRr);

	// Success
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bSuccess = false;

	// Ini File
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FTireConfiguration Configuration;

	// Model Fl
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FModelData ModelFl;

	// Model Fr
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FModelData ModelFr;

	// Model Rl
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FModelData ModelRl;

	// Model Rr
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FModelData ModelRr;
};

/**
 *
 */
UCLASS()
class KARTWORLD_API UTireLoader : public UModLoader
{
	GENERATED_BODY()

private:
	// Getting the FileManager
	IPlatformFile &FileManager = FPlatformFileManager::Get().GetPlatformFile();

	bool GetConfiguration(FString FilePath, FTireConfiguration &TireConfiguration);

public:
	// Mods Directory
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString ModsDir;

	UTireLoader();

	// Initialize loader in game instance
	virtual void Initialize(FString GameUserDir) override;

	// Load track mod by name
	UFUNCTION(BlueprintCallable)
	FTireModel Load(FString TireName);
};
