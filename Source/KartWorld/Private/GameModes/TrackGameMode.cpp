// Copyright @ 2023 Fynn Haupt

#include "GameModes/TrackGameMode.h"

void ATrackGameMode::StartPlay()
{
    Super::StartPlay();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

    // Setup weather and light system
	SkyCreator = World->SpawnActor<ASkyCreator>();

	SkyCreator->RenderMode = EVolumetricCloudRenderTargetMode::VolumetricCloudRenderTargetMode_Performance;
	SkyCreator->MoonPositionType = ESkyCreatorMoonPositionType::MoonPositionType_Real;
	SkyCreator->SunPositionType = ESkyCreatorSunPositionType::SunPositionType_Real;

	TimeComponent = NewObject<UTimeComponent>(SkyCreator, TEXT("Time"));
	TimeComponent->RegisterComponent();
	SkyCreator->AddInstanceComponent(TimeComponent);

	// Setup track
	TrackMesh = World->SpawnActor<ATrackMesh>();
	TrackMesh->TrackName = "ForestFynn";
	TrackMesh->LoadMesh();

	// Setup location
	FTrackConfiguration TrackConfiguration = TrackMesh->GetConfiguration();
	SkyCreator->SetLatitude(TrackConfiguration.Latitude);
	SkyCreator->SetLongitude(TrackConfiguration.Longitude);
	SkyCreator->SetTimeZone(TrackConfiguration.Timezone);
	SkyCreator->SetbDaylightSavingTime(TrackConfiguration.bDst);

	// Setup chassi
	ChassiMesh = World->SpawnActor<AChassiMesh>();
	ChassiMesh->ChassiName = "OTK-DD2";
	ChassiMesh->TireName = "MOJO-D5";
	ChassiMesh->LoadMesh();
}
