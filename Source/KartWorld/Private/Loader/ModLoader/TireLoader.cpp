// Copyright @ 2023 Fynn Haupt


#include "Loader/ModLoader/TireLoader.h"

DEFINE_LOG_CATEGORY(LogTireLoader);

bool UTireLoader::GetConfiguration(FString FilePath, FTireConfiguration &TireConfiguration)
{
    // When Ini file doesn't exist then return failure!
    if (!FileManager.FileExists(*FilePath))
    {
        UE_LOG(LogTireLoader, Error, TEXT("%s.ini - Ini file not found!"), *FilePath);
        return false;
    }

    FIniData IniFile = UIniLibrary::ReadIniFromFile(FilePath);

    // Get sections and when doesn't exist then return failure!
    FIniSection *DetailsSection = IniFile.FindSection(FName("Details"));
    FIniSection *GfxSection = IniFile.FindSection(FName("Gfx"));

    if (DetailsSection == nullptr || GfxSection == nullptr)
    {
        UE_LOG(LogTireLoader, Error, TEXT("%s.ini - Details or Gfx Section not found!"), *FilePath);
        return false;
    }

    if (DetailsSection == nullptr || GfxSection == nullptr)
    {
        UE_LOG(LogTireLoader, Error, TEXT("%s.ini - Details or Gfx Section not found!"), *FilePath);
        return false;
    }

    // Get properties of sections and when doesn't exist then return failure!

    // Details Section
    FIniProperty *NameProperty = DetailsSection->FindProperty(FName("Name"));
    if (NameProperty == nullptr)
    {
        UE_LOG(LogTireLoader, Error, TEXT("%s.ini - Details - Name not found!"), *FilePath);
        return false;
    }
    TireConfiguration.Name = NameProperty->GetValueAsRawString();

    FIniProperty *DescriptionProperty = DetailsSection->FindProperty(FName("Description"));
    if (DescriptionProperty == nullptr)
    {
        UE_LOG(LogTireLoader, Error, TEXT("%s.ini - Details - Description not found!"), *FilePath);
        return false;
    }
    TireConfiguration.Description = DescriptionProperty->GetValueAsRawString();

    FIniProperty *AuthorProperty = DetailsSection->FindProperty(FName("Author"));
    if (NameProperty == nullptr)
    {
        UE_LOG(LogTireLoader, Error, TEXT("%s.ini - Details - Author not found!"), *FilePath);
        return false;
    }
    TireConfiguration.Author = AuthorProperty->GetValueAsRawString();

    // Gfx Section
    FIniProperty *ModelProperty = GfxSection->FindProperty(FName("ModelFl"));
    if (ModelProperty == nullptr)
    {
        UE_LOG(LogTireLoader, Error, TEXT("%s.ini - Gfx - ModelFl not found!"), *FilePath);
        return false;
    }
    TireConfiguration.ModelFl = ModelProperty->GetValueAsRawString();

    FIniProperty *ModelProperty2 = GfxSection->FindProperty(FName("ModelFr"));
    if (ModelProperty2 == nullptr)
    {
        UE_LOG(LogTireLoader, Error, TEXT("%s.ini - Gfx - ModelFr not found!"), *FilePath);
        return false;
    }
    TireConfiguration.ModelFr = ModelProperty2->GetValueAsRawString();

    FIniProperty *ModelProperty3 = GfxSection->FindProperty(FName("ModelRl"));
    if (ModelProperty3 == nullptr)
    {
        UE_LOG(LogTireLoader, Error, TEXT("%s.ini - Gfx - ModelRl not found!"), *FilePath);
        return false;
    }
    TireConfiguration.ModelRl = ModelProperty3->GetValueAsRawString();

    FIniProperty *ModelProperty4 = GfxSection->FindProperty(FName("ModelRr"));
    if (ModelProperty4 == nullptr)
    {
        UE_LOG(LogTireLoader, Error, TEXT("%s.ini - Gfx - ModelRr not found!"), *FilePath);
        return false;
    }
    TireConfiguration.ModelRr = ModelProperty4->GetValueAsRawString();

    return true;
}

UTireLoader::UTireLoader()
{
}

void UTireLoader::Initialize(FString GameUserDir)
{
	Super::Initialize(GameUserDir);

	// Gets game mod directory (eg. Documents/KartSimWorld/mods/Tires), if doesn't exists then create one.
	ModsDir = FPaths::Combine(GameModDir, "Tires");
	if (!FileManager.DirectoryExists(*ModsDir))
		FileManager.CreateDirectory(*ModsDir);
}

FTireModel UTireLoader::Load(FString TireName)
{
	UE_LOG(LogTireLoader, Log, TEXT("Loading Tire %s - Please wait!"), *TireName);

    if(TireName.IsEmpty()) {
        UE_LOG(LogTireLoader, Error, TEXT("Tire name is missing!"));
		return FTireModel();
    }

	FString TireDir = FPaths::Combine(ModsDir, *TireName);

	// When directory doesn't exist then return failure!
	if (!FileManager.DirectoryExists(*TireDir))
	{
		UE_LOG(LogTireLoader, Error, TEXT("Tire %s directory not found!"), *TireName);
		return FTireModel();
	}

	FString IniFileName = FString(TireName + ".ini");
	FString IniFilePath = FPaths::Combine(TireDir, IniFileName);

	// Load configuration
	FTireConfiguration TireConfiguration;
	bool bSuccess = GetConfiguration(IniFilePath, TireConfiguration);
	if (!bSuccess)
		return FTireModel();

	// Load meshes
	FString ModelPathFl = FPaths::Combine(TireDir, TireConfiguration.ModelFl);
	FString ModelPathFr = FPaths::Combine(TireDir, TireConfiguration.ModelFr);
	FString ModelPathRl = FPaths::Combine(TireDir, TireConfiguration.ModelRl);
	FString ModelPathRr = FPaths::Combine(TireDir, TireConfiguration.ModelRr);
	
    FModelData ModelDataFl;
    FModelData ModelDataFr;
    FModelData ModelDataRl;
    FModelData ModelDataRr;
	UMeshLoader *MeshLoader = NewObject<UMeshLoader>();
	MeshLoader->LoadWorld(ModelPathFl, ModelDataFl);
	MeshLoader->LoadWorld(ModelPathFr, ModelDataFr);
	MeshLoader->LoadWorld(ModelPathRl, ModelDataRl);
	MeshLoader->LoadWorld(ModelPathRr, ModelDataRr);

	UE_LOG(LogTireLoader, Log, TEXT("Tire %s was successful loaded!"), *TireName);
	return FTireModel(TireConfiguration, ModelDataFl, ModelDataFr, ModelDataRl, ModelDataRr);
}

FTireModel::FTireModel()
{
}

FTireModel::FTireModel(
    FTireConfiguration Configuration,
		FModelData ModelFl,
		FModelData ModelFr,
		FModelData ModelRl,
		FModelData ModelRr) : 
        bSuccess(true), 
        Configuration(Configuration), 
        ModelFl(ModelFl), 
        ModelFr(ModelFr), 
        ModelRl(ModelRl), 
        ModelRr(ModelRr)
{
}