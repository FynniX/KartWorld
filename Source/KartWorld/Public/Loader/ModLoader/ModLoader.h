// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModLoader.generated.h"

/**
 * 
 */
UCLASS()
class KARTWORLD_API UModLoader : public UObject
{
	GENERATED_BODY()
	
protected:
	// Mods Directory
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString GameModDir;

public:
	UModLoader();

	// Initialize loader in game instance
	UFUNCTION(BlueprintCallable)
	virtual void Initialize(FString GameUserDir);
};
