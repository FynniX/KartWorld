// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Loader/MaterialLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MeshLoader.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMeshLoader, Log, All);

UENUM()
enum EMeshLoadingResult
{
	EMeshLoadingResult_OK = 0,
	EMeshLoadingResult_NOFILE,
	EMeshLoadingResult_NOSCENE,
	EMeshLoadingResult_NOMESHES
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FNodeData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FTransform Transform;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<int32> Meshes;

	TArray<FNodeData> Nodes;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FVertexData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector3f Position;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector3f Normal;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector3f Tangent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FLinearColor Color;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector2f UV0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector2f UV1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector2f UV2;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector2f UV3;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FTriangleData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 UV0 = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 UV1 = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 UV2 = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 PolyGroupIndex = 0;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FLodData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 Lod = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float ScreenSize = 0.0f;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FMeshData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 MaterialId;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FLodData LodData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FVertexData> Verticies;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FTriangleData> Triangles;
};

USTRUCT(BlueprintType)
struct KARTWORLD_API FModelData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FNodeData NodeHierarchy;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FMeshData> Meshes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UMaterialInstanceDynamic *> Materials;
};

/**
 *
 */
UCLASS()
class KARTWORLD_API UMeshLoader : public UObject
{
	GENERATED_BODY()

private:
	// Importer
	Assimp::Importer Importer;

	// Scene
	const aiScene *Scene;

	// Getting the FileManager
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

public:
	UFUNCTION(BlueprintCallable)
	EMeshLoadingResult LoadRelative(
		FString FilePath,
		FModelData &ModelData);

	UFUNCTION(BlueprintCallable)
	EMeshLoadingResult LoadWorld(
		FString FilePath,
		FModelData &ModelData);

private:
	FTransform GetTransformOfNode(aiNode *Node);
	FTransform GetWorldTransformOfNode(aiNode *Node);
	FMatrix GetWorldTransformOfNode(aiNode *Node, FMatrix WorldMatrix);
	FNodeData GetNodeHierarchy(aiNode *Node);
	aiNode* GetParentNode(aiNode* Node, int32 SearchMeshIndex);
	bool GetLodData(FString FilePath, FString SearchMeshName, FLodData& LodData);
};
