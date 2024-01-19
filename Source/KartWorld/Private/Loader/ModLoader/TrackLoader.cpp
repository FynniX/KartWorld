// Copyright @ 2023 Fynn Haupt

#include "Loader/ModLoader/TrackLoader.h"

DEFINE_LOG_CATEGORY(LogTrackLoader);

bool UTrackLoader::GetConfiguration(FString FilePath, FTrackConfiguration &TrackConfiguration)
{
	// When Ini file doesn't exist then return failure!
	if (!FileManager.FileExists(*FilePath))
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Ini file not found!"), *FilePath);
		return false;
	}

	FIniData IniFile = UIniLibrary::ReadIniFromFile(FilePath);

	// Get sections and when doesn't exist then return failure!
	FIniSection *DetailsSection = IniFile.FindSection(FName("Details"));
	FIniSection *LocationSection = IniFile.FindSection(FName("Location"));
	FIniSection *GfxSection = IniFile.FindSection(FName("Gfx"));

	if (DetailsSection == nullptr || LocationSection == nullptr || GfxSection == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Details, Location or Gfx Section not found!"), *FilePath);
		return false;
	}

	if (DetailsSection == nullptr || LocationSection == nullptr || GfxSection == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Details, Location or Gfx Section not found!"), *FilePath);
		return false;
	}

	// Get properties of sections and when doesn't exist then return failure!

	// Details Section
	FIniProperty *NameProperty = DetailsSection->FindProperty(FName("Name"));
	if (NameProperty == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Details - Name not found!"), *FilePath);
		return false;
	}
	TrackConfiguration.Name = NameProperty->GetValueAsRawString();

	FIniProperty *DescriptionProperty = DetailsSection->FindProperty(FName("Description"));
	if (DescriptionProperty == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Details - Description not found!"), *FilePath);
		return false;
	}
	TrackConfiguration.Description = DescriptionProperty->GetValueAsRawString();

	FIniProperty *LengthProperty = DetailsSection->FindProperty(FName("Length"));
	if (LengthProperty == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Details - Length not found!"), *FilePath);
		return false;
	}
	LengthProperty->GetValueAsInt(TrackConfiguration.Length);

	FIniProperty *AltitudeProperty = DetailsSection->FindProperty(FName("Altitude"));
	if (AltitudeProperty == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Details - Altitude not found!"), *FilePath);
		return false;
	}
	AltitudeProperty->GetValueAsInt(TrackConfiguration.Altitude);

	FIniProperty *AuthorProperty = DetailsSection->FindProperty(FName("Author"));
	if (NameProperty == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Details - Author not found!"), *FilePath);
		return false;
	}
	TrackConfiguration.Author = AuthorProperty->GetValueAsRawString();

	// Location Section
	FIniProperty *LatitudeProperty = LocationSection->FindProperty(FName("Latitude"));
	if (LatitudeProperty == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Location - Latitude not found!"), *FilePath);
		return false;
	}
	LatitudeProperty->GetValueAsFloat(TrackConfiguration.Latitude);

	FIniProperty *LongitudeProperty = LocationSection->FindProperty(FName("Longitude"));
	if (LongitudeProperty == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Location - Longitude not found!"), *FilePath);
		return false;
	}
	LongitudeProperty->GetValueAsFloat(TrackConfiguration.Longitude);

	FIniProperty *TimezoneProperty = LocationSection->FindProperty(FName("Timezone"));
	if (TimezoneProperty == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Location - Timezone not found!"), *FilePath);
		return false;
	}
	TimezoneProperty->GetValueAsInt(TrackConfiguration.Timezone);

	FIniProperty *DstProperty = LocationSection->FindProperty(FName("Dst"));
	if (DstProperty == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Location - Dst not found!"), *FilePath);
		return false;
	}
	DstProperty->GetValueAsBoolean(TrackConfiguration.bDst);

	// Gfx Section
	FIniProperty *ModelProperty = GfxSection->FindProperty(FName("Model"));
	if (ModelProperty == nullptr)
	{
		UE_LOG(LogTrackLoader, Error, TEXT("%s.ini - Gfx - Model not found!"), *FilePath);
		return false;
	}

	TrackConfiguration.Model = ModelProperty->GetValueAsRawString();

	return true;
}

UTrackLoader::UTrackLoader()
{
}

void UTrackLoader::Initialize(FString GameUserDir)
{
	Super::Initialize(GameUserDir);

	// Gets game mod directory (eg. Documents/KartSimWorld/mods/Tracks), if doesn't exists then create one.
	ModsDir = FPaths::Combine(GameModDir, "Tracks");
	if (!FileManager.DirectoryExists(*ModsDir))
		FileManager.CreateDirectory(*ModsDir);
}

FTrackModel UTrackLoader::Load(FString TrackName)
{
	UE_LOG(LogTrackLoader, Log, TEXT("Loading Track %s - Please wait!"), *TrackName);

	if(TrackName.IsEmpty()) {
        UE_LOG(LogTrackLoader, Error, TEXT("Track name is missing!"));
		return FTrackModel();
    }

	FString TrackDir = FPaths::Combine(ModsDir, *TrackName);

	// When directory doesn't exist then return failure!
	if (!FileManager.DirectoryExists(*TrackDir))
	{
		UE_LOG(LogTrackLoader, Error, TEXT("Track %s directory not found!"), *TrackName);
		return FTrackModel();
	}

	FString IniFileName = FString(TrackName + ".ini");
	FString IniFilePath = FPaths::Combine(TrackDir, IniFileName);

	// Load configuration
	FTrackConfiguration TrackConfiguration;
	bool bSuccess = GetConfiguration(IniFilePath, TrackConfiguration);
	if (!bSuccess)
		return FTrackModel();

	// Load mesh
	FString ModelPath = FPaths::Combine(TrackDir, TrackConfiguration.Model);
	FModelData ModelData;
	UMeshLoader *MeshLoader = NewObject<UMeshLoader>();
	MeshLoader->LoadWorld(ModelPath, ModelData);

	UE_LOG(LogTrackLoader, Log, TEXT("Track %s was successful loaded!"), *TrackName);
	return FTrackModel(TrackConfiguration, ModelData);
}

FTrackModel::FTrackModel()
{
}

FTrackModel::FTrackModel(
	FTrackConfiguration Configuration,
	FModelData Model) : bSuccess(true), Configuration(Configuration), Model(Model)
{
}
