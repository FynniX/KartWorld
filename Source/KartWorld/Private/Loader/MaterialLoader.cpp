// Copyright @ 2023 Fynn Haupt


#include "Loader/MaterialLoader.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Serialization/JsonReader.h"

DEFINE_LOG_CATEGORY(LogMaterialLoader);

UMaterialLoader::UMaterialLoader()
{
	ConstructorHelpers::FObjectFinder<UMaterial>BaseMetalRoughnessFinder(TEXT("/Script/Engine.Material'/Game/KartWorld/Materials/Base_MetalRoughness.Base_MetalRoughness'"));
	if (BaseMetalRoughnessFinder.Object == nullptr)
		UE_LOG(LogMaterialLoader, Error, TEXT("Base_MetalRoughness - Material can't be found!"));
	BaseMetalRoughness = (UMaterialInstance*)BaseMetalRoughnessFinder.Object;

	ConstructorHelpers::FObjectFinder<UMaterial>BaseSpecGlossFinder(TEXT("/Script/Engine.Material'/Game/KartWorld/Materials/Base_SpecGloss.Base_SpecGloss'"));
	if (BaseSpecGlossFinder.Object == nullptr)
		UE_LOG(LogMaterialLoader, Error, TEXT("Base_SpecGloss - Material can't be found!"));
	BaseSpecGloss = (UMaterialInstance*)BaseSpecGlossFinder.Object;

	ConstructorHelpers::FObjectFinder<UMaterial>BaseMetalRoughnessTilingFinder(TEXT("/Script/Engine.Material'/Game/KartWorld/Materials/Base_MetalRoughness_Tiling.Base_MetalRoughness_Tiling'"));
	if (BaseMetalRoughnessTilingFinder.Object == nullptr)
		UE_LOG(LogMaterialLoader, Error, TEXT("Base_MetalRoughness_Tiling - Material can't be found!"));
	BaseMetalRoughnessTiling = (UMaterialInstance*)BaseMetalRoughnessTilingFinder.Object;

	ConstructorHelpers::FObjectFinder<UMaterial>BaseSpecGlossTilingFinder(TEXT("/Script/Engine.Material'/Game/KartWorld/Materials/Base_SpecGloss_Tiling.Base_SpecGloss_Tiling'"));
	if (BaseSpecGlossTilingFinder.Object == nullptr)
		UE_LOG(LogMaterialLoader, Error, TEXT("Base_SpecGloss_Tiling - Material can't be found!"));
	BaseSpecGlossTiling = (UMaterialInstance*)BaseSpecGlossTilingFinder.Object;
}

TArray<UMaterialInstanceDynamic*> UMaterialLoader::LoadMaterials(FString FolderPath, const aiScene* Scene)
{
	// Reset
	Materials.Empty();
	bSuccess = false;

	if (BaseMetalRoughness == nullptr || BaseSpecGloss == nullptr) {
		UE_LOG(LogMaterialLoader, Error, TEXT("Base materials are missing!"));
		return Materials;
	}

	// Setup array length
	NumMaterials = (int32)Scene->mNumMaterials;
	Materials.SetNum(NumMaterials);

	for (int32 MaterialIndex = 0; MaterialIndex < NumMaterials; MaterialIndex++) {
		aiMaterial* Material = Scene->mMaterials[MaterialIndex];

		if (Material->GetTextureCount(aiTextureType_DIFFUSE) == 0) {
			UE_LOG(LogMaterialLoader, Warning, TEXT("%s - Has no diffuse texture!"), *FString(Material->GetName().C_Str()));
			continue;
		}

		// Get diffuse texture path
		aiString MatkeyResult;
		Material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), MatkeyResult);
		FString TexturePath(MatkeyResult.C_Str());
		if (!FPaths::FileExists(TexturePath))
			TexturePath = FPaths::Combine(FolderPath, TexturePath);
		FPaths::MakePlatformFilename(TexturePath);
		FPaths::CollapseRelativeDirectories(TexturePath);

		if (!FPaths::FileExists(TexturePath)) {
			UE_LOG(LogMaterialLoader, Warning, TEXT("%s - Diffuse texture can't be found!"), *FString(Material->GetName().C_Str()));
			continue;
		}

		// Split path to get material file path
		FString TextureFolder;
		FString TextureName;
		FString TextureExtension;
		FPaths::Split(TexturePath, TextureFolder, TextureName, TextureExtension);

		FString MaterialFileName(TextureName + ".mat");
		FString MaterialFilePath = FPaths::Combine(TextureFolder, MaterialFileName);

		EMaterialBase UsedMaterialBase = EMaterialBase_MetalRoughness;
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		if (FPaths::FileExists(MaterialFilePath)) {
			// Load Json
			FString MaterialFileContent;
			FFileHelper::LoadFileToString(MaterialFileContent, *MaterialFilePath);
			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(MaterialFileContent);
			FJsonSerializer::Deserialize(JsonReader, JsonObject);

			if (!JsonObject.IsValid()) {
				UE_LOG(LogMaterialLoader, Error, TEXT("%s - Material file can't be read!"), *FString(Material->GetName().C_Str()));
				continue;
			}

			// Change to used material base
			if (JsonObject->TryGetField("Mode") != nullptr) {
				int32 Mode = JsonObject->GetIntegerField("Mode");

				switch (Mode) {
				case EMaterialBase_SpecGloss:
					UsedMaterialBase = EMaterialBase_SpecGloss;
					break;
				case EMaterialBase_MetalRoughness_Tiling:
					UsedMaterialBase = EMaterialBase_MetalRoughness_Tiling;
					break;
				case EMaterialBase_SpecGloss_Tiling:
					UsedMaterialBase = EMaterialBase_SpecGloss_Tiling;
					break;
				}
			}
		}
		
		// Generate Material
		UMaterialInstanceDynamic* MaterialInstance;
		switch (UsedMaterialBase) {
			case EMaterialBase_SpecGloss:
				MaterialInstance = GenerateSpecGlossMaterial(TexturePath, Material, JsonObject);
				break;
			case EMaterialBase_MetalRoughness_Tiling:
				MaterialInstance = GenerateMetalRoughnessTilingMaterial(TexturePath, Material, JsonObject);
				break;
			case EMaterialBase_SpecGloss_Tiling:
				MaterialInstance = GenerateSpecGlossTilingMaterial(TexturePath, Material, JsonObject);
				break;
			default: 
				MaterialInstance = GenerateMetalRoughnessMaterial(TexturePath, Material, JsonObject);
				break;
		}
	
		// Set Material Instance
		Materials[MaterialIndex] = MaterialInstance;
	}

	// Loading was successful
	bSuccess = true;
	return Materials;
}

UMaterialInstanceDynamic* UMaterialLoader::GenerateMetalRoughnessMaterial(FString BaseColorPath, aiMaterial* Material, TSharedPtr<FJsonObject> JsonObject)
{
	// Split path to get base color file path
	FString TextureFolder;
	FString TextureName;
	FString TextureExtension;
	FPaths::Split(BaseColorPath, TextureFolder, TextureName, TextureExtension);

	UMaterialInstanceDynamic* MaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), BaseMetalRoughness);
	UDirectDrawSurfaceLoader* DirectDrawSurfaceLoader = NewObject<UDirectDrawSurfaceLoader>();

	// BaseColor
	EErrorCode Error = DirectDrawSurfaceLoader->LoadTexture(BaseColorPath);
	if (Error == EErrorCode_OK)
		MaterialInstance->SetTextureParameterValue("BaseColor", DirectDrawSurfaceLoader->TextureArray[0]);

	if (!JsonObject.IsValid() || JsonObject->TryGetField("Maps") == nullptr) {
		if (Error == EErrorCode_OK) {
			UE_LOG(LogMaterialLoader, Warning, TEXT("%s - Maps section missing in material file, therefore only adding base color!"), *FString(Material->GetName().C_Str()));
			return MaterialInstance;
		}
		else {
			UE_LOG(LogMaterialLoader, Error, TEXT("%s - Maps section missing in material file, but no base color was found!"), *FString(Material->GetName().C_Str()));
			return nullptr;
		}
	}

	TSharedPtr<FJsonObject> MapsObject = JsonObject->GetObjectField("Maps");

	// OcclusionRoughnessMetallic
	if (MapsObject->TryGetField("OcclusionRoughnessMetallic") != nullptr) {
		TSharedPtr<FJsonObject> OcclusionRoughnessMetallicObject = MapsObject->GetObjectField("OcclusionRoughnessMetallic");

		if (OcclusionRoughnessMetallicObject->TryGetField("OcclusionStrength") != nullptr) {
			float OcclusionStrength = (float)OcclusionRoughnessMetallicObject->GetNumberField("OcclusionStrength");
			MaterialInstance->SetScalarParameterValue("OcclusionStrength", OcclusionStrength);
		}

		if (OcclusionRoughnessMetallicObject->TryGetField("RoughnessStrength") != nullptr) {
			float RoughnessStrength = (float)OcclusionRoughnessMetallicObject->GetNumberField("RoughnessStrength");
			MaterialInstance->SetScalarParameterValue("RoughnessStrength", RoughnessStrength);
		}

		if (OcclusionRoughnessMetallicObject->TryGetField("MetallicStrength") != nullptr) {
			float MetallicStrength = (float)OcclusionRoughnessMetallicObject->GetNumberField("MetallicStrength");
			MaterialInstance->SetScalarParameterValue("MetallicStrength", MetallicStrength);
		}

		if (OcclusionRoughnessMetallicObject->TryGetField("Path") != nullptr) {
			FString Path = OcclusionRoughnessMetallicObject->GetStringField("Path");
			if (!FPaths::FileExists(Path))
				Path = FPaths::Combine(TextureFolder, Path);
			FPaths::MakePlatformFilename(Path);
			FPaths::CollapseRelativeDirectories(Path);
			Error = DirectDrawSurfaceLoader->LoadTexture(Path);
			if (Error == EErrorCode_OK)
				MaterialInstance->SetTextureParameterValue("OcclusionRoughnessMetallic", DirectDrawSurfaceLoader->TextureArray[0]);
		}
	}

	// Normal
	if (MapsObject->TryGetField("Normal") != nullptr) {
		TSharedPtr<FJsonObject> NormalObject = MapsObject->GetObjectField("Normal");

		if (NormalObject->TryGetField("Strength") != nullptr) {
			float Strength = (float)NormalObject->GetNumberField("Strength");
			MaterialInstance->SetScalarParameterValue("NormalStrength", Strength);
		}

		if (NormalObject->TryGetField("Path") != nullptr) {
			FString Path = NormalObject->GetStringField("Path");
			if (!FPaths::FileExists(Path))
				Path = FPaths::Combine(TextureFolder, Path);
			FPaths::MakePlatformFilename(Path);
			FPaths::CollapseRelativeDirectories(Path);
			Error = DirectDrawSurfaceLoader->LoadTexture(Path);
			if (Error == EErrorCode_OK)
				MaterialInstance->SetTextureParameterValue("Normal", DirectDrawSurfaceLoader->TextureArray[0]);
		}
	}

	// Emissive
	if (MapsObject->TryGetField("Emissive") != nullptr) {
		TSharedPtr<FJsonObject> EmissiveObject = MapsObject->GetObjectField("Emissive");

		if (EmissiveObject->TryGetField("Strength") != nullptr) {
			float Strength = (float)EmissiveObject->GetNumberField("Strength");
			MaterialInstance->SetScalarParameterValue("EmissiveStrength", Strength);
		}
		else {
			MaterialInstance->SetScalarParameterValue("UseAmbientOcclusion", 1.0);
		}

		if (EmissiveObject->TryGetField("Path") != nullptr) {
			FString Path = EmissiveObject->GetStringField("Path");
			if (!FPaths::FileExists(Path))
				Path = FPaths::Combine(TextureFolder, Path);
			FPaths::MakePlatformFilename(Path);
			FPaths::CollapseRelativeDirectories(Path);
			Error = DirectDrawSurfaceLoader->LoadTexture(Path);
			if (Error == EErrorCode_OK)
				MaterialInstance->SetTextureParameterValue("Emissive", DirectDrawSurfaceLoader->TextureArray[0]);
		} 
		else {
			MaterialInstance->SetScalarParameterValue("UseAmbientOcclusion", 1.0);
		}
	}
	else {
		MaterialInstance->SetScalarParameterValue("UseAmbientOcclusion", 1.0);
	}
	
	return MaterialInstance;
}

UMaterialInstanceDynamic* UMaterialLoader::GenerateSpecGlossMaterial(FString DiffusePath, aiMaterial* Material, TSharedPtr<FJsonObject> JsonObject)
{
	// Split path to get base color file path
	FString TextureFolder;
	FString TextureName;
	FString TextureExtension;
	FPaths::Split(DiffusePath, TextureFolder, TextureName, TextureExtension);

	UMaterialInstanceDynamic* MaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), BaseSpecGloss);
	UDirectDrawSurfaceLoader* DirectDrawSurfaceLoader = NewObject<UDirectDrawSurfaceLoader>();

	// Diffuse
	EErrorCode Error = DirectDrawSurfaceLoader->LoadTexture(DiffusePath);
	if (Error == EErrorCode_OK)
		MaterialInstance->SetTextureParameterValue("Diffuse", DirectDrawSurfaceLoader->TextureArray[0]);

	if (!JsonObject.IsValid() || JsonObject->TryGetField("Maps") == nullptr) {
		if (Error == EErrorCode_OK) {
			UE_LOG(LogMaterialLoader, Warning, TEXT("%s - Maps section missing in material file, therefore only adding diffuse!"), *FString(Material->GetName().C_Str()));
			return MaterialInstance;
		}
		else {
			UE_LOG(LogMaterialLoader, Error, TEXT("%s - Maps section missing in material file, but no diffuse was found!"), *FString(Material->GetName().C_Str()));
			return nullptr;
		}
	}

	TSharedPtr<FJsonObject> MapsObject = JsonObject->GetObjectField("Maps");

	// SpecGloss
	if (MapsObject->TryGetField("SpecGloss") != nullptr) {
		TSharedPtr<FJsonObject> SpecGlossObject = MapsObject->GetObjectField("SpecGloss");

		if (SpecGlossObject->TryGetField("SpecStrength") != nullptr) {
			float SpecStrength = (float)SpecGlossObject->GetNumberField("SpecStrength");
			MaterialInstance->SetScalarParameterValue("SpecStrength", SpecStrength);
		}

		if (SpecGlossObject->TryGetField("GlossStrength") != nullptr) {
			float GlossStrength = (float)SpecGlossObject->GetNumberField("GlossStrength");
			MaterialInstance->SetScalarParameterValue("GlossStrength", GlossStrength);
		}

		if (SpecGlossObject->TryGetField("Path") != nullptr) {
			FString Path = SpecGlossObject->GetStringField("Path");
			if (!FPaths::FileExists(Path))
				Path = FPaths::Combine(TextureFolder, Path);
			FPaths::MakePlatformFilename(Path);
			FPaths::CollapseRelativeDirectories(Path);
			Error = DirectDrawSurfaceLoader->LoadTexture(Path);
			if (Error == EErrorCode_OK)
				MaterialInstance->SetTextureParameterValue("SpecGloss", DirectDrawSurfaceLoader->TextureArray[0]);
		}
	}

	// Normal
	if (MapsObject->TryGetField("Normal") != nullptr) {
		TSharedPtr<FJsonObject> NormalObject = MapsObject->GetObjectField("Normal");

		if (NormalObject->TryGetField("Strength") != nullptr) {
			float Strength = (float)NormalObject->GetNumberField("Strength");
			MaterialInstance->SetScalarParameterValue("NormalStrength", Strength);
		}

		if (NormalObject->TryGetField("Path") != nullptr) {
			FString Path = NormalObject->GetStringField("Path");
			if (!FPaths::FileExists(Path))
				Path = FPaths::Combine(TextureFolder, Path);
			FPaths::MakePlatformFilename(Path);
			FPaths::CollapseRelativeDirectories(Path);
			Error = DirectDrawSurfaceLoader->LoadTexture(Path);
			if (Error == EErrorCode_OK)
				MaterialInstance->SetTextureParameterValue("Normal", DirectDrawSurfaceLoader->TextureArray[0]);
		}
	}

	return MaterialInstance;
}

UMaterialInstanceDynamic* UMaterialLoader::GenerateMetalRoughnessTilingMaterial(FString BaseColorPath, aiMaterial* Material, TSharedPtr<FJsonObject> JsonObject)
{
	// Split path to get base color file path
	FString TextureFolder;
	FString TextureName;
	FString TextureExtension;
	FPaths::Split(BaseColorPath, TextureFolder, TextureName, TextureExtension);

	UMaterialInstanceDynamic* MaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), BaseMetalRoughnessTiling);
	UDirectDrawSurfaceLoader* DirectDrawSurfaceLoader = NewObject<UDirectDrawSurfaceLoader>();

	// BaseColor
	EErrorCode Error = DirectDrawSurfaceLoader->LoadTexture(BaseColorPath);
	if (Error == EErrorCode_OK)
		MaterialInstance->SetTextureParameterValue("BaseColor", DirectDrawSurfaceLoader->TextureArray[0]);

	if (!JsonObject.IsValid() || JsonObject->TryGetField("Maps") == nullptr) {
		if (Error == EErrorCode_OK) {
			UE_LOG(LogMaterialLoader, Warning, TEXT("%s - Maps section missing in material file, therefore only adding base color!"), *FString(Material->GetName().C_Str()));
			return MaterialInstance;
		}
		else {
			UE_LOG(LogMaterialLoader, Error, TEXT("%s - Maps section missing in material file, but no base color was found!"), *FString(Material->GetName().C_Str()));
			return nullptr;
		}
	}

	TSharedPtr<FJsonObject> MapsObject = JsonObject->GetObjectField("Maps");

	// TilingScale
	if (MapsObject->TryGetField("TilingScale") != nullptr) {
		float TilingScale = (float)MapsObject->GetNumberField("TilingScale");
		MaterialInstance->SetScalarParameterValue("TilingScale", TilingScale);
	}

	// TilingVariation
	if (MapsObject->TryGetField("TilingVariation") != nullptr) {
		float TilingVariation = (float)MapsObject->GetNumberField("TilingVariation");
		MaterialInstance->SetScalarParameterValue("TilingVariation", TilingVariation);
	}

	// OcclusionRoughnessMetallic
	if (MapsObject->TryGetField("OcclusionRoughnessMetallic") != nullptr) {
		TSharedPtr<FJsonObject> OcclusionRoughnessMetallicObject = MapsObject->GetObjectField("OcclusionRoughnessMetallic");

		if (OcclusionRoughnessMetallicObject->TryGetField("OcclusionStrength") != nullptr) {
			float OcclusionStrength = (float)OcclusionRoughnessMetallicObject->GetNumberField("OcclusionStrength");
			MaterialInstance->SetScalarParameterValue("OcclusionStrength", OcclusionStrength);
		}

		if (OcclusionRoughnessMetallicObject->TryGetField("RoughnessStrength") != nullptr) {
			float RoughnessStrength = (float)OcclusionRoughnessMetallicObject->GetNumberField("RoughnessStrength");
			MaterialInstance->SetScalarParameterValue("RoughnessStrength", RoughnessStrength);
		}

		if (OcclusionRoughnessMetallicObject->TryGetField("MetallicStrength") != nullptr) {
			float MetallicStrength = (float)OcclusionRoughnessMetallicObject->GetNumberField("MetallicStrength");
			MaterialInstance->SetScalarParameterValue("MetallicStrength", MetallicStrength);
		}

		if (OcclusionRoughnessMetallicObject->TryGetField("Path") != nullptr) {
			FString Path = OcclusionRoughnessMetallicObject->GetStringField("Path");
			if (!FPaths::FileExists(Path))
				Path = FPaths::Combine(TextureFolder, Path);
			FPaths::MakePlatformFilename(Path);
			FPaths::CollapseRelativeDirectories(Path);
			Error = DirectDrawSurfaceLoader->LoadTexture(Path);
			if (Error == EErrorCode_OK)
				MaterialInstance->SetTextureParameterValue("OcclusionRoughnessMetallic", DirectDrawSurfaceLoader->TextureArray[0]);
		}
	}

	// Normal
	if (MapsObject->TryGetField("Normal") != nullptr) {
		TSharedPtr<FJsonObject> NormalObject = MapsObject->GetObjectField("Normal");

		if (NormalObject->TryGetField("Strength") != nullptr) {
			float Strength = (float)NormalObject->GetNumberField("Strength");
			MaterialInstance->SetScalarParameterValue("NormalStrength", Strength);
		}

		if (NormalObject->TryGetField("Path") != nullptr) {
			FString Path = NormalObject->GetStringField("Path");
			if (!FPaths::FileExists(Path))
				Path = FPaths::Combine(TextureFolder, Path);
			FPaths::MakePlatformFilename(Path);
			FPaths::CollapseRelativeDirectories(Path);
			Error = DirectDrawSurfaceLoader->LoadTexture(Path);
			if (Error == EErrorCode_OK)
				MaterialInstance->SetTextureParameterValue("Normal", DirectDrawSurfaceLoader->TextureArray[0]);
		}
	}

	// Emissive
	if (MapsObject->TryGetField("Emissive") != nullptr) {
		TSharedPtr<FJsonObject> EmissiveObject = MapsObject->GetObjectField("Emissive");

		if (EmissiveObject->TryGetField("Strength") != nullptr) {
			float Strength = (float)EmissiveObject->GetNumberField("Strength");
			MaterialInstance->SetScalarParameterValue("EmissiveStrength", Strength);
		}
		else {
			MaterialInstance->SetScalarParameterValue("UseAmbientOcclusion", 1.0);
		}

		if (EmissiveObject->TryGetField("Path") != nullptr) {
			FString Path = EmissiveObject->GetStringField("Path");
			if (!FPaths::FileExists(Path))
				Path = FPaths::Combine(TextureFolder, Path);
			FPaths::MakePlatformFilename(Path);
			FPaths::CollapseRelativeDirectories(Path);
			Error = DirectDrawSurfaceLoader->LoadTexture(Path);
			if (Error == EErrorCode_OK)
				MaterialInstance->SetTextureParameterValue("Emissive", DirectDrawSurfaceLoader->TextureArray[0]);
		}
		else {
			MaterialInstance->SetScalarParameterValue("UseAmbientOcclusion", 1.0);
		}
	}
	else {
		MaterialInstance->SetScalarParameterValue("UseAmbientOcclusion", 1.0);
	}
	return MaterialInstance;
}

UMaterialInstanceDynamic* UMaterialLoader::GenerateSpecGlossTilingMaterial(FString DiffusePath, aiMaterial* Material, TSharedPtr<FJsonObject> JsonObject)
{
	// Split path to get base color file path
	FString TextureFolder;
	FString TextureName;
	FString TextureExtension;
	FPaths::Split(DiffusePath, TextureFolder, TextureName, TextureExtension);

	UMaterialInstanceDynamic* MaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), BaseSpecGlossTiling);
	UDirectDrawSurfaceLoader* DirectDrawSurfaceLoader = NewObject<UDirectDrawSurfaceLoader>();

	// Diffuse
	EErrorCode Error = DirectDrawSurfaceLoader->LoadTexture(DiffusePath);
	if (Error == EErrorCode_OK)
		MaterialInstance->SetTextureParameterValue("Diffuse", DirectDrawSurfaceLoader->TextureArray[0]);

	if (!JsonObject.IsValid() || JsonObject->TryGetField("Maps") == nullptr) {
		if (Error == EErrorCode_OK) {
			UE_LOG(LogMaterialLoader, Warning, TEXT("%s - Maps section missing in material file, therefore only adding diffuse!"), *FString(Material->GetName().C_Str()));
			return MaterialInstance;
		}
		else {
			UE_LOG(LogMaterialLoader, Error, TEXT("%s - Maps section missing in material file, but no diffuse was found!"), *FString(Material->GetName().C_Str()));
			return nullptr;
		}
	}

	TSharedPtr<FJsonObject> MapsObject = JsonObject->GetObjectField("Maps");

	// TilingScale
	if (MapsObject->TryGetField("TilingScale") != nullptr) {
		float TilingScale = (float)MapsObject->GetNumberField("TilingScale");
		MaterialInstance->SetScalarParameterValue("TilingScale", TilingScale);
	}

	// TilingVariation
	if (MapsObject->TryGetField("TilingVariation") != nullptr) {
		float TilingVariation = (float)MapsObject->GetNumberField("TilingVariation");
		MaterialInstance->SetScalarParameterValue("TilingVariation", TilingVariation);
	}

	// SpecGloss
	if (MapsObject->TryGetField("SpecGloss") != nullptr) {
		TSharedPtr<FJsonObject> SpecGlossObject = MapsObject->GetObjectField("SpecGloss");

		if (SpecGlossObject->TryGetField("SpecStrength") != nullptr) {
			float SpecStrength = (float)SpecGlossObject->GetNumberField("SpecStrength");
			MaterialInstance->SetScalarParameterValue("SpecStrength", SpecStrength);
		}

		if (SpecGlossObject->TryGetField("GlossStrength") != nullptr) {
			float GlossStrength = (float)SpecGlossObject->GetNumberField("GlossStrength");
			MaterialInstance->SetScalarParameterValue("GlossStrength", GlossStrength);
		}

		if (SpecGlossObject->TryGetField("Path") != nullptr) {
			FString Path = SpecGlossObject->GetStringField("Path");
			if (!FPaths::FileExists(Path))
				Path = FPaths::Combine(TextureFolder, Path);
			FPaths::MakePlatformFilename(Path);
			FPaths::CollapseRelativeDirectories(Path);
			Error = DirectDrawSurfaceLoader->LoadTexture(Path);
			if (Error == EErrorCode_OK)
				MaterialInstance->SetTextureParameterValue("SpecGloss", DirectDrawSurfaceLoader->TextureArray[0]);
		}
	}

	// Normal
	if (MapsObject->TryGetField("Normal") != nullptr) {
		TSharedPtr<FJsonObject> NormalObject = MapsObject->GetObjectField("Normal");

		if (NormalObject->TryGetField("Strength") != nullptr) {
			float Strength = (float)NormalObject->GetNumberField("Strength");
			MaterialInstance->SetScalarParameterValue("NormalStrength", Strength);
		}

		if (NormalObject->TryGetField("Path") != nullptr) {
			FString Path = NormalObject->GetStringField("Path");
			if (!FPaths::FileExists(Path))
				Path = FPaths::Combine(TextureFolder, Path);
			FPaths::MakePlatformFilename(Path);
			FPaths::CollapseRelativeDirectories(Path);
			Error = DirectDrawSurfaceLoader->LoadTexture(Path);
			if (Error == EErrorCode_OK)
				MaterialInstance->SetTextureParameterValue("Normal", DirectDrawSurfaceLoader->TextureArray[0]);
		}
	}

	return MaterialInstance;
}