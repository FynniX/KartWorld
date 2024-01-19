// Copyright @ 2023 Fynn Haupt

#pragma once

#include "CoreMinimal.h"
#include "Loader/TextureLoader/TextureLoader.h"
#include "DirextXTex/DDS.h"
#include "DirectDrawSurfaceLoader.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDirectDrawLoader, Log, All);

using namespace DirectX;

/**
 * 
 */
UCLASS()
class KARTWORLD_API UDirectDrawSurfaceLoader : public UTextureLoader
{
	GENERATED_BODY()

private:
	const uint16 MaxSize = 8192;

	uint32 BitsPerPixel(DXGI_FORMAT Format);
	DXGI_FORMAT GetDxgiFormat(const DDS_PIXELFORMAT& ddpf);
	EPixelFormat GetPixelFormat(DXGI_FORMAT Format);
	EErrorCode GetSurfaceData(uint16 BaseWidth, uint16 BaseHeight, DXGI_FORMAT Format, uint32& OutNumBytes, uint32& OutRowBytes, uint32& OutNumRows);
	EErrorCode InterpretFile(uint32& MagicValue, DDS_HEADER& Header, DDS_HEADER_DXT10& Header10);
	EErrorCode GetResourceDimension(
		uint16& BaseWidth, uint16& BaseHeight, uint16& BaseDepth, 
		uint32& ArraySize, uint32& MipCount, bool& bIsCubeMap,
		DXGI_FORMAT& Format, D3D12_RESOURCE_DIMENSION& ResDim,
		DDS_HEADER& Header, DDS_HEADER_DXT10& Header10
	);
	EErrorCode IsResourceDimensionSupported(
		uint16& BaseWidth, uint16& BaseHeight, uint16& BaseDepth, 
		uint32& ArraySize, bool& bIsCubeMap, D3D12_RESOURCE_DIMENSION& ResDim
	);

public:
	virtual EErrorCode LoadTexture(FString FilePath) override;
};
