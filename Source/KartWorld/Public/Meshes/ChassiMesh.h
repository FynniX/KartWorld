// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "RealtimeMeshComponent.h"
#include "RealtimeMeshSimple.h"
#include "MainGameInstance.h"
#include "ChassiMesh.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChassiMesh, Log, All);

UCLASS()
class KARTWORLD_API AChassiMesh : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UMainGameInstance *GameInstance;

	UPROPERTY()
	FChassiModel ChassiModel;

	UPROPERTY()
	FTireModel TireModel;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ChassiName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TireName;

	// Sets default values for this actor's properties
	AChassiMesh();

	UFUNCTION(BlueprintCallable)
	void LoadMesh();

	UFUNCTION(BlueprintCallable)
	inline FChassiConfiguration GetConfiguration() const { return ChassiModel.Configuration; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	USceneComponent* CreateNode(FNodeData& NodeData);
	URealtimeMeshComponent* CreateMesh(int32 MeshIndex);
	URealtimeMeshComponent* CreateTireModel(FModelData& ModelData);
};
