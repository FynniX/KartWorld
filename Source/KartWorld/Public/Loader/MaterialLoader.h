// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Loader/TextureLoader/DirectDrawSurfaceLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MaterialLoader.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMaterialLoader, Log, All);

UENUM()
enum EMaterialBase {
	EMaterialBase_MetalRoughness		UMETA(DisplayName = "MetalRoughness"),
	EMaterialBase_SpecGloss				UMETA(DisplayName = "SpecGloss"),
	EMaterialBase_MetalRoughness_Tiling	UMETA(DisplayName = "MetalRoughness_Tiling"),
	EMaterialBase_SpecGloss_Tiling		UMETA(DisplayName = "SpecGloss_Tiling")
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FOcclusionRoughnessMetallicMap {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float OcclusionStrength;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float RoughnessStrength;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float MetallicStrength;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Path;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FSpecGlossMap {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float SpecStrength;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float GlossStrength;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Path;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FNormalMap {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Strength;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Path;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FEmissiveMap {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float Strength;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Path;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FMetalRoughnessJson {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FOcclusionRoughnessMetallicMap OcclusionRoughnessMetallicMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FNormalMap NormalMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FEmissiveMap EmissiveMap;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FSpecGlossJson {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FSpecGlossMap SpecGlossMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FNormalMap NormalMap;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FMetalRoughnessTilingJson {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float TilingScale = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FOcclusionRoughnessMetallicMap OcclusionRoughnessMetallicMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FNormalMap NormalMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FEmissiveMap EmissiveMap;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FSpecGlossTilingJson {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float TilingScale = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FSpecGlossMap SpecGlossMap;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FNormalMap NormalMap;
};

/**
 * 
 */
UCLASS()
class KARTWORLD_API UMaterialLoader : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UMaterialInstance* BaseMetalRoughness;

	UPROPERTY()
	UMaterialInstance* BaseSpecGloss;

	UPROPERTY()
	UMaterialInstance* BaseMetalRoughnessTiling;

	UPROPERTY()
	UMaterialInstance* BaseSpecGlossTiling;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bSuccess = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 NumMaterials = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UMaterialInstanceDynamic*> Materials;

	UMaterialLoader();

	TArray<UMaterialInstanceDynamic*> LoadMaterials(FString FolderPath, const aiScene* Scene);

private:
	UMaterialInstanceDynamic* GenerateMetalRoughnessMaterial(FString BaseColorPath, aiMaterial* Material, TSharedPtr<FJsonObject> JsonObject);

	UMaterialInstanceDynamic* GenerateSpecGlossMaterial(FString DiffusePath, aiMaterial* Material, TSharedPtr<FJsonObject> JsonObject);

	UMaterialInstanceDynamic* GenerateMetalRoughnessTilingMaterial(FString BaseColorPath, aiMaterial* Material, TSharedPtr<FJsonObject> JsonObject);

	UMaterialInstanceDynamic* GenerateSpecGlossTilingMaterial(FString DiffusePath, aiMaterial* Material, TSharedPtr<FJsonObject> JsonObject);
};
