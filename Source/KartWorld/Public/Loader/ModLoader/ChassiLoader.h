// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "Loader/ModLoader/ModLoader.h"
#include "Loader/MeshLoader.h"
#include "IniLibrary.h"
#include "ChassiLoader.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChassiLoader, Log, All);

USTRUCT(BlueprintType)
struct KARTWORLD_API FSteeringColumn {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString SteeringWheel;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString SteeringRodLeft;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString SteeringRodRight;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FStubAxleLeft {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString SteeringRod;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Wheel;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FStubAxleRight {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString SteeringRod;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Wheel;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FRearAxle {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString WheelLeft;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString WheelRight;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FChassiLinking {
	GENERATED_BODY()

public:
	// SteeringColumn
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "SteeringColumn")
	FSteeringColumn SteeringColumn;

	// StubAxleLeft
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "StubAxleLeft")
	FStubAxleLeft StubAxleLeft;

	// StubAxleRight
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "StubAxleRight")
	FStubAxleRight StubAxleRight;

	// RearAxle
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RearAxle")
	FRearAxle RearAxle;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FChassiConfiguration {
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
	FString Model;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gfx")
	FString Linking;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FChassiModel {
	GENERATED_BODY()

public:
	FChassiModel();
	FChassiModel(FChassiConfiguration Configuration, FChassiLinking Linking, FModelData Model);

	// Success
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bSuccess = false;

	// Ini File
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FChassiConfiguration Configuration;

	// Linking File
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FChassiLinking Linking;

	// Model
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FModelData Model;
};

/**
 * 
 */
UCLASS()
class KARTWORLD_API UChassiLoader : public UModLoader
{
	GENERATED_BODY()
	
private:
	// Getting the FileManager
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	bool GetLinking(FString FilePath, FChassiLinking& ChassiLinking);
	bool GetConfiguration(FString FilePath, FChassiConfiguration& ChassiConfiguration);
	
public:
	// Mods Directory
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString ModsDir;

	UChassiLoader();

	// Initialize loader in game instance
	virtual void Initialize(FString GameUserDir) override;

	// Load track mod by name
	UFUNCTION(BlueprintCallable)
	FChassiModel Load(FString TrackName);
};
