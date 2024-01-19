// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class KartWorld : ModuleRules
{
	private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
    }

	public KartWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json", "IniParser", "SkyCreatorPlugin", "RealtimeMeshComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// DDSTextureLoader
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "DirextXTex/include"));

        // Assimp
        string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "Win64" : "Win32";
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "assimp/include"));
        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "assimp/lib", PlatformString, "assimp-vc143-mt.lib"));
        RuntimeDependencies.Add("assimp-vc143-mt.dll");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
