// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TextureLoader.generated.h"

UENUM()
enum EErrorCode {
	EErrorCode_OK = 0,
	EErrorCode_NOFILE,
	EErrorCode_FAILED,
	EErrorCode_CORRUPT,
	EErrorCode_NOTSUPPORTED,
	EErrorCode_INVALIDDATA,
	EErrorCode_OVERFLOW
};

/**
 * 
 */
UCLASS()
class KARTWORLD_API UTextureLoader : public UObject
{
	GENERATED_BODY()

protected:
	TArray<uint8> FileArray;
	int32 FileSize = 0;
	uint8* FileBuffer;
	int32 ReadOffset = 0;

	template <typename T>
	FORCEINLINE T ReadFromBuffer() {
		T Data = *((T*)(FileBuffer + ReadOffset));
		ReadOffset += sizeof(T);
		return Data;
	}
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString TextureFolder;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString TextureName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FString TextureExtension;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UTexture*> TextureArray;

	UFUNCTION(BlueprintCallable)
	virtual EErrorCode LoadTexture(FString FilePath);

	UTextureLoader();
};
