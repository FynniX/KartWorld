// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "KartMesh.generated.h"

/**
 * 
 */
UCLASS()
class KARTWORLD_API AKartMesh : public ASkeletalMeshActor
{
	GENERATED_BODY()
	
public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
