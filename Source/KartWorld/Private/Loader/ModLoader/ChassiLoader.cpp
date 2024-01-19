// Copyright @ 2023 Fynn Haupt

#include "Loader/ModLoader/ChassiLoader.h"

DEFINE_LOG_CATEGORY(LogChassiLoader);

bool UChassiLoader::GetLinking(FString FilePath, FChassiLinking &ChassiLinking)
{
    // When Link file doesn't exist then return
    if (!FileManager.FileExists(*FilePath))
    {
        // Is no issue so dont say anything
        // UE_LOG(LogChassiLoader, Warning, TEXT("%s - Link file not found!"), *FilePath);
        return false;
    }

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    // Load Json
    FString JsonFileContent;
    FFileHelper::LoadFileToString(JsonFileContent, *FilePath);
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonFileContent);
    FJsonSerializer::Deserialize(JsonReader, JsonObject);

    if (!JsonObject.IsValid())
    {
        UE_LOG(LogChassiLoader, Error, TEXT("%s - Link file can't be read!"), *FilePath);
        return false;
    }

    if (JsonObject->TryGetField("SteeringColumn") != nullptr)
    {
        TSharedPtr<FJsonObject> SteeringColumnObject = JsonObject->GetObjectField("SteeringColumn");

        if (SteeringColumnObject->TryGetField("Name") != nullptr)
        {
            ChassiLinking.SteeringColumn.Name = SteeringColumnObject->GetStringField("Name");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - SteeringColumn - Name missing!"), *FilePath);
            return false;
        }

        if (SteeringColumnObject->TryGetField("SteeringWheel") != nullptr)
        {
            ChassiLinking.SteeringColumn.SteeringWheel = SteeringColumnObject->GetStringField("SteeringWheel");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - SteeringColumn - SteeringWheel missing!"), *FilePath);
            return false;
        }

        if (SteeringColumnObject->TryGetField("SteeringRodLeft") != nullptr)
        {
            ChassiLinking.SteeringColumn.SteeringRodLeft = SteeringColumnObject->GetStringField("SteeringRodLeft");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - SteeringColumn - SteeringRodLeft missing!"), *FilePath);
            return false;
        }

        if (SteeringColumnObject->TryGetField("SteeringRodRight") != nullptr)
        {
            ChassiLinking.SteeringColumn.SteeringRodRight = SteeringColumnObject->GetStringField("SteeringRodRight");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - SteeringColumn - SteeringRodRight missing!"), *FilePath);
            return false;
        }
    }

    if (JsonObject->TryGetField("StubAxleLeft") != nullptr)
    {
        TSharedPtr<FJsonObject> StubAxleLeftObject = JsonObject->GetObjectField("StubAxleLeft");

        if (StubAxleLeftObject->TryGetField("Name") != nullptr)
        {
            ChassiLinking.SteeringColumn.Name = StubAxleLeftObject->GetStringField("Name");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - StubAxleLeft - Name missing!"), *FilePath);
            return false;
        }

        if (StubAxleLeftObject->TryGetField("SteeringRod") != nullptr)
        {
            ChassiLinking.StubAxleLeft.SteeringRod = StubAxleLeftObject->GetStringField("SteeringRod");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - StubAxleLeft - SteeringRod missing!"), *FilePath);
            return false;
        }

        if (StubAxleLeftObject->TryGetField("Wheel") != nullptr)
        {
            ChassiLinking.StubAxleLeft.Wheel = StubAxleLeftObject->GetStringField("Wheel");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - StubAxleLeft - Wheel missing!"), *FilePath);
            return false;
        }
    }

    if (JsonObject->TryGetField("StubAxleRight") != nullptr)
    {
        TSharedPtr<FJsonObject> StubAxleRightObject = JsonObject->GetObjectField("StubAxleRight");

        if (StubAxleRightObject->TryGetField("Name") != nullptr)
        {
            ChassiLinking.StubAxleRight.Name = StubAxleRightObject->GetStringField("Name");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - StubAxleRight - Name missing!"), *FilePath);
            return false;
        }

        if (StubAxleRightObject->TryGetField("SteeringRod") != nullptr)
        {
            ChassiLinking.StubAxleRight.SteeringRod = StubAxleRightObject->GetStringField("SteeringRod");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - StubAxleRight - SteeringRod missing!"), *FilePath);
            return false;
        }

        if (StubAxleRightObject->TryGetField("Wheel") != nullptr)
        {
            ChassiLinking.StubAxleRight.Wheel = StubAxleRightObject->GetStringField("Wheel");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - StubAxleRight - Wheel missing!"), *FilePath);
            return false;
        }
    }

    if (JsonObject->TryGetField("RearAxle") != nullptr)
    {
        TSharedPtr<FJsonObject> RearAxleObject = JsonObject->GetObjectField("RearAxle");

        if (RearAxleObject->TryGetField("Name") != nullptr)
        {
            ChassiLinking.RearAxle.Name = RearAxleObject->GetStringField("Name");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - RearAxle - Name missing!"), *FilePath);
            return false;
        }

        if (RearAxleObject->TryGetField("WheelLeft") != nullptr)
        {
            ChassiLinking.RearAxle.WheelLeft = RearAxleObject->GetStringField("WheelLeft");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - RearAxle - WheelLeft missing!"), *FilePath);
            return false;
        }

        if (RearAxleObject->TryGetField("WheelRight") != nullptr)
        {
            ChassiLinking.RearAxle.WheelRight = RearAxleObject->GetStringField("WheelRight");
        }
        else
        {
            UE_LOG(LogChassiLoader, Error, TEXT("%s - RearAxle - WheelRight missing!"), *FilePath);
            return false;
        }
    }

    return true;
}

bool UChassiLoader::GetConfiguration(FString FilePath, FChassiConfiguration &ChassiConfiguration)
{
    // When Ini file doesn't exist then return failure!
    if (!FileManager.FileExists(*FilePath))
    {
        UE_LOG(LogChassiLoader, Error, TEXT("%s.ini - Ini file not found!"), *FilePath);
        return false;
    }

    FIniData IniFile = UIniLibrary::ReadIniFromFile(FilePath);

    // Get sections and when doesn't exist then return failure!
    FIniSection *DetailsSection = IniFile.FindSection(FName("Details"));
    FIniSection *GfxSection = IniFile.FindSection(FName("Gfx"));

    if (DetailsSection == nullptr || GfxSection == nullptr)
    {
        UE_LOG(LogChassiLoader, Error, TEXT("%s.ini - Details or Gfx Section not found!"), *FilePath);
        return false;
    }

    if (DetailsSection == nullptr || GfxSection == nullptr)
    {
        UE_LOG(LogChassiLoader, Error, TEXT("%s.ini - Details or Gfx Section not found!"), *FilePath);
        return false;
    }

    // Get properties of sections and when doesn't exist then return failure!

    // Details Section
    FIniProperty *NameProperty = DetailsSection->FindProperty(FName("Name"));
    if (NameProperty == nullptr)
    {
        UE_LOG(LogChassiLoader, Error, TEXT("%s.ini - Details - Name not found!"), *FilePath);
        return false;
    }
    ChassiConfiguration.Name = NameProperty->GetValueAsRawString();

    FIniProperty *DescriptionProperty = DetailsSection->FindProperty(FName("Description"));
    if (DescriptionProperty == nullptr)
    {
        UE_LOG(LogChassiLoader, Error, TEXT("%s.ini - Details - Description not found!"), *FilePath);
        return false;
    }
    ChassiConfiguration.Description = DescriptionProperty->GetValueAsRawString();

    FIniProperty *AuthorProperty = DetailsSection->FindProperty(FName("Author"));
    if (NameProperty == nullptr)
    {
        UE_LOG(LogChassiLoader, Error, TEXT("%s.ini - Details - Author not found!"), *FilePath);
        return false;
    }
    ChassiConfiguration.Author = AuthorProperty->GetValueAsRawString();

    // Gfx Section
    FIniProperty *ModelProperty = GfxSection->FindProperty(FName("Model"));
    if (ModelProperty == nullptr)
    {
        UE_LOG(LogChassiLoader, Error, TEXT("%s.ini - Gfx - Model not found!"), *FilePath);
        return false;
    }

    ChassiConfiguration.Model = ModelProperty->GetValueAsRawString();

    FIniProperty *ModelProperty2 = GfxSection->FindProperty(FName("Linking"));
    if (ModelProperty2 == nullptr)
    {
        UE_LOG(LogChassiLoader, Error, TEXT("%s.ini - Gfx - Linking not found!"), *FilePath);
        return false;
    }

    ChassiConfiguration.Linking = ModelProperty2->GetValueAsRawString();

    return true;
}

UChassiLoader::UChassiLoader()
{
}

void UChassiLoader::Initialize(FString GameUserDir)
{
    Super::Initialize(GameUserDir);

    // Gets game mod directory (eg. Documents/KartSimWorld/mods/Chassis), if doesn't exists then create one.
    ModsDir = FPaths::Combine(GameModDir, "Chassis");
    if (!FileManager.DirectoryExists(*ModsDir))
        FileManager.CreateDirectory(*ModsDir);
}

FChassiModel UChassiLoader::Load(FString ChassiName)
{
    UE_LOG(LogChassiLoader, Log, TEXT("Loading Chassi %s - Please wait!"), *ChassiName);

    if (ChassiName.IsEmpty())
    {
        UE_LOG(LogChassiLoader, Error, TEXT("Chassi name is missing!"));
        return FChassiModel();
    }

    FString ChassiDir = FPaths::Combine(ModsDir, *ChassiName);

    // When directory doesn't exist then return failure!
    if (!FileManager.DirectoryExists(*ChassiDir))
    {
        UE_LOG(LogChassiLoader, Error, TEXT("Track %s directory not found!"), *ChassiName);
        return FChassiModel();
    }

    FString IniFileName = FString(ChassiName + ".ini");
    FString IniFilePath = FPaths::Combine(ChassiDir, IniFileName);

    // Load configuration
    FChassiConfiguration ChassiConfiguration;
    bool bSuccess = GetConfiguration(IniFilePath, ChassiConfiguration);
    if (!bSuccess)
        return FChassiModel();

    // Load linking
    FString LinkingPath = FPaths::Combine(ChassiDir, ChassiConfiguration.Linking);
    FChassiLinking ChassiLinking;
    bSuccess = GetLinking(LinkingPath, ChassiLinking);
    if (!bSuccess)
        return FChassiModel();

    // Load mesh
    FString ModelPath = FPaths::Combine(ChassiDir, ChassiConfiguration.Model);
    FModelData ModelData;
    UMeshLoader *MeshLoader = NewObject<UMeshLoader>();
    MeshLoader->LoadRelative(ModelPath, ModelData);

    UE_LOG(LogChassiLoader, Log, TEXT("Chassi %s was successful loaded!"), *ChassiName);
    return FChassiModel(ChassiConfiguration, ChassiLinking, ModelData);
}

FChassiModel::FChassiModel()
{
}

FChassiModel::FChassiModel(
    FChassiConfiguration Configuration,
    FChassiLinking Linking,
    FModelData Model) : bSuccess(true),
                        Configuration(Configuration),
                        Linking(Linking),
                        Model(Model)
{
}
