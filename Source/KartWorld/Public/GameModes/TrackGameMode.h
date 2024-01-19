// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SkyCreatorActor.h"
#include "WeatherSystem/TimeComponent.h"
#include "Meshes/TrackMesh.h"
#include "Meshes/ChassiMesh.h"
#include "TrackGameMode.generated.h"

/**
 *
 */
UCLASS()
class KARTWORLD_API ATrackGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ASkyCreator* SkyCreator;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UTimeComponent* TimeComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ATrackMesh* TrackMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AChassiMesh* ChassiMesh;

	virtual void StartPlay() override;
};
