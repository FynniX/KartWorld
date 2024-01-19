// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RealtimeMeshComponent.h"
#include "RealtimeMeshSimple.h"
#include "MainGameInstance.h"
#include "TrackMesh.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTrackMesh, Log, All);

UCLASS()
class KARTWORLD_API ATrackMesh : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UMainGameInstance *GameInstance;

	UPROPERTY()
	URealtimeMeshSimple* Mesh;

	UPROPERTY()
	FTrackModel TrackModel;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	URealtimeMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TrackName;

	// Sets default values for this actor's properties
	ATrackMesh();

	UFUNCTION(BlueprintCallable)
	void LoadMesh();

	UFUNCTION(BlueprintCallable)
	inline FTrackConfiguration GetConfiguration() const { return TrackModel.Configuration; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	USceneComponent* CreateNode(FNodeData& NodeData);
	URealtimeMeshComponent* CreateMesh(int32 MeshIndex);
};
