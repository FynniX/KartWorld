// Copyright @ 2023 Fynn Haupt


#include "Loader/TextureLoader/DirectDrawSurfaceLoader.h"

DEFINE_LOG_CATEGORY(LogDirectDrawLoader);

uint32 UDirectDrawSurfaceLoader::BitsPerPixel(DXGI_FORMAT Format)
{
	switch (Format)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 128;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 96;

	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
	case DXGI_FORMAT_Y416:
	case DXGI_FORMAT_Y210:
	case DXGI_FORMAT_Y216:
		return 64;

	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
	case DXGI_FORMAT_AYUV:
	case DXGI_FORMAT_Y410:
	case DXGI_FORMAT_YUY2:
		return 32;

	case DXGI_FORMAT_P010:
	case DXGI_FORMAT_P016:
	case DXGI_FORMAT_V408:
		return 24;

	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
	case DXGI_FORMAT_A8P8:
	case DXGI_FORMAT_B4G4R4A4_UNORM:
	case DXGI_FORMAT_P208:
	case DXGI_FORMAT_V208:
		return 16;

	case DXGI_FORMAT_NV12:
	case DXGI_FORMAT_420_OPAQUE:
	case DXGI_FORMAT_NV11:
		return 12;

	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
	case DXGI_FORMAT_AI44:
	case DXGI_FORMAT_IA44:
	case DXGI_FORMAT_P8:
		return 8;

	case DXGI_FORMAT_R1_UNORM:
		return 1;

	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		return 4;

	default:
		return 0;
	}
}

DXGI_FORMAT UDirectDrawSurfaceLoader::GetDxgiFormat(const DDS_PIXELFORMAT& ddpf)
{
	if (ddpf.flags & DDS_RGB)
	{
		// Note that sRGB formats are written using the "DX10" extended header

		switch (ddpf.RGBBitCount)
		{
		case 32:
			if (ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
			{
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000))
			{
				return DXGI_FORMAT_B8G8R8A8_UNORM;
			}

			if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0))
			{
				return DXGI_FORMAT_B8G8R8X8_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0) aka D3DFMT_X8B8G8R8

			// Note that many common DDS reader/writers (including D3DX) swap the
			// the RED/BLUE masks for 10:10:10:2 formats. We assume
			// below that the 'backwards' header mask is being used since it is most
			// likely written by D3DX. The more robust solution is to use the 'DX10'
			// header extension and specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly

			// For 'correct' writers, this should be 0x000003ff,0x000ffc00,0x3ff00000 for RGB data
			if (ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000))
			{
				return DXGI_FORMAT_R10G10B10A2_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) aka D3DFMT_A2R10G10B10

			if (ISBITMASK(0x0000ffff, 0xffff0000, 0, 0))
			{
				return DXGI_FORMAT_R16G16_UNORM;
			}

			if (ISBITMASK(0xffffffff, 0, 0, 0))
			{
				// Only 32-bit color channel format in D3D9 was R32F
				return DXGI_FORMAT_R32_FLOAT; // D3DX writes this out as a FourCC of 114
			}
			break;

		case 24:
			// No 24bpp DXGI formats aka D3DFMT_R8G8B8
			break;

		case 16:
			if (ISBITMASK(0x7c00, 0x03e0, 0x001f, 0x8000))
			{
				return DXGI_FORMAT_B5G5R5A1_UNORM;
			}
			if (ISBITMASK(0xf800, 0x07e0, 0x001f, 0))
			{
				return DXGI_FORMAT_B5G6R5_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x7c00,0x03e0,0x001f,0) aka D3DFMT_X1R5G5B5

			if (ISBITMASK(0x0f00, 0x00f0, 0x000f, 0xf000))
			{
				return DXGI_FORMAT_B4G4R4A4_UNORM;
			}

			// NVTT versions 1.x wrote this as RGB instead of LUMINANCE
			if (ISBITMASK(0x00ff, 0, 0, 0xff00))
			{
				return DXGI_FORMAT_R8G8_UNORM;
			}
			if (ISBITMASK(0xffff, 0, 0, 0))
			{
				return DXGI_FORMAT_R16_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x0f00,0x00f0,0x000f,0) aka D3DFMT_X4R4G4B4

			// No 3:3:2:8 or paletted DXGI formats aka D3DFMT_A8R3G3B2, D3DFMT_A8P8, etc.
			break;

		case 8:
			// NVTT versions 1.x wrote this as RGB instead of LUMINANCE
			if (ISBITMASK(0xff, 0, 0, 0))
			{
				return DXGI_FORMAT_R8_UNORM;
			}

			// No 3:3:2 or paletted DXGI formats aka D3DFMT_R3G3B2, D3DFMT_P8
			break;
		}
	}
	else if (ddpf.flags & DDS_LUMINANCE)
	{
		switch (ddpf.RGBBitCount)
		{
		case 16:
			if (ISBITMASK(0xffff, 0, 0, 0))
			{
				return DXGI_FORMAT_R16_UNORM; // D3DX10/11 writes this out as DX10 extension
			}
			if (ISBITMASK(0x00ff, 0, 0, 0xff00))
			{
				return DXGI_FORMAT_R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension
			}
			break;

		case 8:
			if (ISBITMASK(0xff, 0, 0, 0))
			{
				return DXGI_FORMAT_R8_UNORM; // D3DX10/11 writes this out as DX10 extension
			}

			// No DXGI format maps to ISBITMASK(0x0f,0,0,0xf0) aka D3DFMT_A4L4

			if (ISBITMASK(0x00ff, 0, 0, 0xff00))
			{
				return DXGI_FORMAT_R8G8_UNORM; // Some DDS writers assume the bitcount should be 8 instead of 16
			}
			break;
		}
	}
	else if (ddpf.flags & DDS_ALPHA)
	{
		if (8 == ddpf.RGBBitCount)
		{
			return DXGI_FORMAT_A8_UNORM;
		}
	}
	else if (ddpf.flags & DDS_BUMPDUDV)
	{
		switch (ddpf.RGBBitCount)
		{
		case 32:
			if (ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
			{
				return DXGI_FORMAT_R8G8B8A8_SNORM; // D3DX10/11 writes this out as DX10 extension
			}
			if (ISBITMASK(0x0000ffff, 0xffff0000, 0, 0))
			{
				return DXGI_FORMAT_R16G16_SNORM; // D3DX10/11 writes this out as DX10 extension
			}

			// No DXGI format maps to ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000) aka D3DFMT_A2W10V10U10
			break;

		case 16:
			if (ISBITMASK(0x00ff, 0xff00, 0, 0))
			{
				return DXGI_FORMAT_R8G8_SNORM; // D3DX10/11 writes this out as DX10 extension
			}
			break;
		}

		// No DXGI format maps to DDPF_BUMPLUMINANCE aka D3DFMT_L6V5U5, D3DFMT_X8L8V8U8
	}
	else if (ddpf.flags & DDS_FOURCC)
	{
		if (MAKEFOURCC('D', 'X', 'T', '1') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC1_UNORM;
		}
		if (MAKEFOURCC('D', 'X', 'T', '3') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC2_UNORM;
		}
		if (MAKEFOURCC('D', 'X', 'T', '5') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC3_UNORM;
		}

		// While pre-multiplied alpha isn't directly supported by the DXGI formats,
		// they are basically the same as these BC formats so they can be mapped
		if (MAKEFOURCC('D', 'X', 'T', '2') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC2_UNORM;
		}
		if (MAKEFOURCC('D', 'X', 'T', '4') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC3_UNORM;
		}

		if (MAKEFOURCC('A', 'T', 'I', '1') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '4', 'U') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '4', 'S') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_SNORM;
		}

		if (MAKEFOURCC('A', 'T', 'I', '2') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '5', 'U') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '5', 'S') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_SNORM;
		}

		// BC6H and BC7 are written using the "DX10" extended header

		if (MAKEFOURCC('R', 'G', 'B', 'G') == ddpf.fourCC)
		{
			return DXGI_FORMAT_R8G8_B8G8_UNORM;
		}
		if (MAKEFOURCC('G', 'R', 'G', 'B') == ddpf.fourCC)
		{
			return DXGI_FORMAT_G8R8_G8B8_UNORM;
		}

		if (MAKEFOURCC('Y', 'U', 'Y', '2') == ddpf.fourCC)
		{
			return DXGI_FORMAT_YUY2;
		}

		// Check for D3DFORMAT enums being set here
		switch (ddpf.fourCC)
		{
		case 36: // D3DFMT_A16B16G16R16
			return DXGI_FORMAT_R16G16B16A16_UNORM;

		case 110: // D3DFMT_Q16W16V16U16
			return DXGI_FORMAT_R16G16B16A16_SNORM;

		case 111: // D3DFMT_R16F
			return DXGI_FORMAT_R16_FLOAT;

		case 112: // D3DFMT_G16R16F
			return DXGI_FORMAT_R16G16_FLOAT;

		case 113: // D3DFMT_A16B16G16R16F
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case 114: // D3DFMT_R32F
			return DXGI_FORMAT_R32_FLOAT;

		case 115: // D3DFMT_G32R32F
			return DXGI_FORMAT_R32G32_FLOAT;

		case 116: // D3DFMT_A32B32G32R32F
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

			// No DXGI format maps to D3DFMT_CxV8U8
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}

EPixelFormat UDirectDrawSurfaceLoader::GetPixelFormat(DXGI_FORMAT Format)
{
	// TODO: Add other formats
	switch (Format) {
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case  DXGI_FORMAT_BC1_UNORM_SRGB:
		return PF_DXT1;
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
		return PF_DXT3;
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
		return PF_DXT5;
	case DXGI_FORMAT_BC4_TYPELESS:
	case  DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		return PF_BC4;
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
		return PF_BC5;
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
		return PF_BC6H;
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		return PF_BC7;
	default:
		return PF_Unknown;
	}
}

EErrorCode UDirectDrawSurfaceLoader::GetSurfaceData(uint16 BaseWidth, uint16 BaseHeight, DXGI_FORMAT Format, uint32& OutNumBytes, uint32& OutRowBytes, uint32& OutNumRows)
{
	bool bBc = false;
	bool bPacked = false;
	bool bPlanar = false;
	uint32 Bpe = 0;

	switch (Format) {
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		bBc = true;
		Bpe = 8;
		break;

	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		bBc = true;
		Bpe = 16;
		break;

	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_YUY2:
		bPacked = true;
		Bpe = 4;
		break;

	case DXGI_FORMAT_Y210:
	case DXGI_FORMAT_Y216:
		bPacked = true;
		Bpe = 8;
		break;

	case DXGI_FORMAT_NV12:
	case DXGI_FORMAT_420_OPAQUE:
		if ((BaseHeight % 2) != 0) {
			// Requires a height alignment of 2.
			UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
			return EErrorCode_NOTSUPPORTED;
		}

		bPlanar = true;
		Bpe = 2;
		break;

	case DXGI_FORMAT_P208:
		bPlanar = true;
		Bpe = 2;
		break;

	case DXGI_FORMAT_P010:
	case DXGI_FORMAT_P016:
		if ((BaseHeight % 2) != 0) {
			// Requires a height alignment of 2.
			UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
			return EErrorCode_NOTSUPPORTED;
		}

		bPlanar = true;
		Bpe = 4;
		break;

	default:
		break;
	}

	if (bBc)
	{
		uint64_t numBlocksWide = 0;
		if (BaseWidth > 0)
		{
			numBlocksWide = std::max<uint64_t>(1u, (uint64_t(BaseWidth) + 3u) / 4u);
		}
		uint64_t numBlocksHigh = 0;
		if (BaseHeight > 0)
		{
			numBlocksHigh = std::max<uint64_t>(1u, (uint64_t(BaseHeight) + 3u) / 4u);
		}
		OutRowBytes = numBlocksWide * Bpe;
		OutNumRows = numBlocksHigh;
		OutNumBytes = OutRowBytes * numBlocksHigh;
	}
	else if (bPacked)
	{
		OutRowBytes = ((uint64_t(BaseWidth) + 1u) >> 1) * Bpe;
		OutNumRows = uint64_t(BaseHeight);
		OutNumBytes = OutRowBytes * BaseHeight;
	}
	else if (Format == DXGI_FORMAT_NV11)
	{
		OutRowBytes = ((uint64_t(BaseWidth) + 3u) >> 2) * 4u;
		OutNumRows = uint64_t(BaseHeight) * 2u; // Direct3D makes this simplifying assumption, although it is larger than the 4:1:1 data
		OutNumBytes = OutRowBytes * OutNumRows;
	}
	else if (bPlanar)
	{
		OutRowBytes = ((uint64_t(BaseWidth) + 1u) >> 1) * Bpe;
		OutNumBytes = (OutRowBytes * uint64_t(BaseHeight)) + ((OutRowBytes * uint64_t(BaseHeight) + 1u) >> 1);
		OutNumRows = BaseHeight + ((uint64_t(BaseHeight) + 1u) >> 1);
	}
	else
	{
		const uint32 Bpp = BitsPerPixel(Format);
		if (!Bpp) {
			UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
			return EErrorCode_NOTSUPPORTED;
		}

		OutRowBytes = (uint64_t(BaseWidth) * Bpp + 7u) / 8u; // round up to nearest byte
		OutNumRows = uint64_t(BaseHeight);
		OutNumBytes = OutRowBytes * BaseHeight;
	}

#if defined(_M_IX86) || defined(_M_ARM) || defined(_M_HYBRID_X86_ARM64)
	if (OutNumBytes > UINT32_MAX || OutRowBytes > UINT32_MAX || numRows > UINT32_MAX) {
		UE_LOG(LogDirectDrawLoader, Error, TEXT("FillInitData - Arithmetic overflow!"));
		return EErrorCode_OVERFLOW;
	}
#endif

	return EErrorCode_OK;
}

EErrorCode UDirectDrawSurfaceLoader::InterpretFile(uint32& MagicValue, DDS_HEADER& Header, DDS_HEADER_DXT10& Header10)
{
	MagicValue = ReadFromBuffer<uint32>();

	if (MagicValue != DDS_MAGIC) {
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File is corrupt!"), *TextureName, *TextureExtension);
		return EErrorCode_CORRUPT;
	}

	Header = ReadFromBuffer<DDS_HEADER>();

	// Verify header to validate DDS file
	if (Header.size != sizeof(DDS_HEADER) || Header.ddspf.size != sizeof(DDS_PIXELFORMAT))
	{
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File is corrupt!"), *TextureName, *TextureExtension);
		return EErrorCode_CORRUPT;
	}

	// Check for DX10 extension
	bool bDxt10Header = false;
	if ((Header.ddspf.flags & DDS_FOURCC) && (MAKEFOURCC('D', 'X', '1', '0') == Header.ddspf.fourCC))
	{
		// Must be long enough for both headers and magic value
		if (FileSize < (sizeof(uint32) + sizeof(DDS_HEADER) + sizeof(DDS_HEADER_DXT10)))
		{
			UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File is corrupt!"), *TextureName, *TextureExtension);
			return EErrorCode_CORRUPT;
		}

		bDxt10Header = true;
		Header10 = ReadFromBuffer<DDS_HEADER_DXT10>();
	}

	return EErrorCode_OK;
}

EErrorCode UDirectDrawSurfaceLoader::GetResourceDimension(
	uint16& BaseWidth, uint16& BaseHeight, uint16& BaseDepth,
	uint32& ArraySize, uint32& MipCount, bool& bIsCubeMap,
	DXGI_FORMAT& Format, D3D12_RESOURCE_DIMENSION& ResDim,
	DDS_HEADER& Header, DDS_HEADER_DXT10& Header10
)
{
	// DX10 Formats
	if ((Header.ddspf.flags & DDS_FOURCC) && (MAKEFOURCC('D', 'X', '1', '0') == Header.ddspf.fourCC)) {
		ArraySize = Header10.arraySize == 0 ? 1 : Header10.arraySize;

		switch (Header10.dxgiFormat) {
		case DXGI_FORMAT_NV12:
		case DXGI_FORMAT_P010:
		case DXGI_FORMAT_P016:
		case DXGI_FORMAT_420_OPAQUE:
			// Planar formats are not supported!
			UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
			return EErrorCode_NOTSUPPORTED;

		case DXGI_FORMAT_YUY2:
		case DXGI_FORMAT_Y210:
		case DXGI_FORMAT_Y216:
		case DXGI_FORMAT_P208:
			if ((BaseWidth % 2) != 0)
			{
				UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
				return EErrorCode_NOTSUPPORTED;
			}
			break;

		case DXGI_FORMAT_NV11:
			if ((BaseWidth % 4) != 0)
			{
				UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
				return EErrorCode_NOTSUPPORTED;
			}
			break;

		case DXGI_FORMAT_AI44:
		case DXGI_FORMAT_IA44:
		case DXGI_FORMAT_P8:
		case DXGI_FORMAT_A8P8:
			UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
			return EErrorCode_NOTSUPPORTED;

		case DXGI_FORMAT_V208:
			if ((Header10.resourceDimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D) || (BaseHeight % 2) != 0)
			{
				UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
				return EErrorCode_NOTSUPPORTED;
			}
			break;

		default:
			if (BitsPerPixel(Header10.dxgiFormat) == 0)
			{
				UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
				return EErrorCode_NOTSUPPORTED;
			}
		}

		Format = Header10.dxgiFormat;

		switch (Header10.resourceDimension) {
		case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
			// D3DX writes 1D textures with a fixed Height of 1
			if ((Header.flags & DDS_HEIGHT) && BaseHeight != 1)
			{
				UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File has invalid data!"), *TextureName, *TextureExtension);
				return EErrorCode_INVALIDDATA;
			}
			BaseHeight = BaseDepth = 1;
			break;

		case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
			if (Header10.miscFlag & 0x4 /* RESOURCE_MISC_TEXTURECUBE */)
			{
				ArraySize *= 6;
				bIsCubeMap = true;
			}
			BaseDepth = 1;
			break;

		case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
			if (!(Header.flags & DDS_HEADER_FLAGS_VOLUME))
			{
				UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File has invalid data!"), *TextureName, *TextureExtension);
				return EErrorCode_INVALIDDATA;
			}

			if (ArraySize > 1)
			{
				UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
				return EErrorCode_NOTSUPPORTED;
			}
			break;

		default:
			UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
			return EErrorCode_NOTSUPPORTED;
		}

		ResDim = static_cast<D3D12_RESOURCE_DIMENSION>(Header10.resourceDimension);

		return EErrorCode_OK;
	}

	// DXGI Formats

	Format = GetDxgiFormat(Header.ddspf);

	if (Format == DXGI_FORMAT_UNKNOWN) {
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
		return EErrorCode_NOTSUPPORTED;
	}

	if (Header.flags & DDS_HEADER_FLAGS_VOLUME) {
		ResDim = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	}
	else {
		if (Header.caps2 & DDS_CUBEMAP) {
			// We require all six faces to be defined
			if ((Header.caps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES) {
				UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
				return EErrorCode_NOTSUPPORTED;
			}

			ArraySize = 6;
			bIsCubeMap = true;
		}

		BaseDepth = 1;
		ResDim = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		// Note there's no way for a legacy Direct3D 9 DDS to express a '1D' texture
	}

	if (BitsPerPixel(Format) == 0) {
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
		return EErrorCode_NOTSUPPORTED;
	}

	return EErrorCode_OK;
}

EErrorCode UDirectDrawSurfaceLoader::IsResourceDimensionSupported(
	uint16& BaseWidth, uint16& BaseHeight, uint16& BaseDepth,
	uint32& ArraySize, bool& bIsCubeMap, D3D12_RESOURCE_DIMENSION& ResDim
)
{
	switch (ResDim)
	{
	case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
		if ((ArraySize > D3D12_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION) ||
			(BaseWidth > D3D12_REQ_TEXTURE1D_U_DIMENSION))
		{
			UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
			return EErrorCode_NOTSUPPORTED;
		}
		break;

	case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
		if (bIsCubeMap)
		{
			// This is the right bound because we set arraySize to (NumCubes*6) above
			if ((ArraySize > D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
				(BaseWidth > D3D12_REQ_TEXTURECUBE_DIMENSION) ||
				(BaseHeight > D3D12_REQ_TEXTURECUBE_DIMENSION))
			{
				UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
				return EErrorCode_NOTSUPPORTED;
			}
		}
		else if ((ArraySize > D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
			(BaseWidth > D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION) ||
			(BaseHeight > D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION))
		{
			UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
			return EErrorCode_NOTSUPPORTED;
		}
		break;

	case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
		if ((ArraySize > 1) ||
			(BaseWidth > D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
			(BaseHeight > D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
			(BaseDepth > D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION))
		{
			UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
			return EErrorCode_NOTSUPPORTED;
		}
		break;

	default:
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
		return EErrorCode_NOTSUPPORTED;
	}

	return EErrorCode_OK;
}

EErrorCode UDirectDrawSurfaceLoader::LoadTexture(FString FilePath)
{
	// Split file path into components
	FPaths::Split(FilePath, TextureFolder, TextureName, TextureExtension);

	UE_LOG(LogDirectDrawLoader, Log, TEXT("%s.%s - Loading!"), *TextureName, *TextureExtension);

	// Reset everything
	ReadOffset = 0;
	TextureArray.Empty();
	FileArray.Empty();

	// Check weather file exists.
	if (!FPaths::FileExists(FilePath)) {
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - Doesn't exist!"), *TextureName, *TextureExtension);
		return EErrorCode_NOFILE;
	}

	// Load file
	if (!FFileHelper::LoadFileToArray(FileArray, *FilePath))
	{
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - Failed to load file!"), *TextureName, *TextureExtension);
		return EErrorCode_FAILED;
	}

	FileSize = FileArray.Num();
	FileBuffer = FileArray.GetData();

	// Check weather size is correct
	if ((FileSize > UINT32_MAX) || (FileSize < (sizeof(uint32) + sizeof(DDS_HEADER))))
	{
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File is corrupt!"), *TextureName, *TextureExtension);
		return EErrorCode_CORRUPT;
	}

	// Interpret File
	uint32 MagicValue;
	DDS_HEADER Header;
	DDS_HEADER_DXT10 Header10;
	EErrorCode ErrorCode = InterpretFile(MagicValue, Header, Header10);

	if (ErrorCode != EErrorCode_OK)
		return EErrorCode_CORRUPT;

	// Image data
	uint32 BitSize = FileSize - ReadOffset;
	uint8* BitData = FileBuffer + ReadOffset;

	// Texture Data
	uint16 BaseWidth = Header.width;
	uint16 BaseHeight = Header.height;
	uint16 BaseDepth = Header.depth;
	uint32 ArraySize = 1;
	bool bIsCubeMap = false;
	DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;
	D3D12_RESOURCE_DIMENSION ResDim = D3D12_RESOURCE_DIMENSION_UNKNOWN;
	uint32 MipCount = Header.mipMapCount == 0 ? 1 : Header.mipMapCount;

	// Get resource dimension
	GetResourceDimension(
		BaseWidth, BaseHeight, BaseDepth,
		ArraySize, MipCount, bIsCubeMap,
		Format, ResDim, Header, Header10
	);

	// Bound sizes (for security purposes we don't trust DDS file metadata larger than the Direct3D hardware requirements)
	if (MipCount > D3D12_REQ_MIP_LEVELS)
	{
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
		return EErrorCode_NOTSUPPORTED;
	}

	// TODO: Cube Support
	if (bIsCubeMap) {
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
		return EErrorCode_NOTSUPPORTED;
	}

	ErrorCode = IsResourceDimensionSupported(
		BaseWidth, BaseHeight, BaseDepth,
		ArraySize, bIsCubeMap, ResDim
	);

	if (ErrorCode != EErrorCode_OK)
		return EErrorCode_NOTSUPPORTED;

	EPixelFormat PixelFormat = GetPixelFormat(Format);

	// TODO: Add support for more formats
	if (PixelFormat == PF_Unknown) {
		UE_LOG(LogDirectDrawLoader, Error, TEXT("%s.%s - File format not supported!"), *TextureName, *TextureExtension);
		return EErrorCode_NOTSUPPORTED;
	}

	uint32 NumBytes = 0;
	uint32 RowBytes = 0;
	uint32 NumRows = 0;

	TextureArray.SetNum(ArraySize);
	uint8* SrcBits = BitData;
	uint32 SrcSize = BitSize;
	for (uint32 ArrayIndex = 0; ArrayIndex < ArraySize; ArrayIndex++)
	{
		uint16 TextureWidth = BaseWidth;
		uint16 TextureHeight = BaseHeight;
		uint16 TextureDepth = BaseDepth;
		UTexture2D* Texture = UTexture2D::CreateTransient(TextureWidth, TextureHeight, PixelFormat, FName(TextureName));
		Texture->CompressionNone = true;
		Texture->UpdateResource();

		for (uint32 MipLevel = 0; MipLevel < MipCount; MipLevel++)
		{
			GetSurfaceData(TextureWidth, TextureHeight, Format, NumBytes, RowBytes, NumRows);
			//UE_LOG(LogDirectDrawLoader, Log, TEXT("%d, %d, %d"), MipLevel, TextureWidth, TextureHeight);

			if (MipLevel == 0) {
				FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];

				Mip.BulkData.Lock(LOCK_READ_WRITE);
				Mip.BulkData.Realloc((int64)NumBytes);
				Mip.BulkData.Unlock();

				void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(Data, SrcBits, (int64)NumBytes);
				Mip.BulkData.Unlock();
			}
			else {
				FTexture2DMipMap* Mip = new FTexture2DMipMap(TextureWidth, TextureHeight, TextureDepth);
				Texture->GetPlatformData()->Mips.Add(Mip);

				Mip->BulkData.Lock(LOCK_READ_WRITE);
				Mip->BulkData.Realloc((int64)NumBytes);
				Mip->BulkData.Unlock();

				void* Data = Mip->BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(Data, SrcBits, (int64)NumBytes);
				Mip->BulkData.Unlock();
			}

			// Add offset for next map
			SrcBits += NumBytes * TextureDepth;
			SrcSize -= NumBytes * TextureDepth;

			// Each mip map size gets smaller by multiplier 0.5
			TextureWidth = TextureWidth >> 1;
			TextureHeight = TextureHeight >> 1;
			TextureDepth = TextureDepth >> 1;

			// Parameters can't be 0!
			if (TextureWidth == 0) TextureWidth = 1;
			if (TextureHeight == 0) TextureHeight = 1;
			if (TextureDepth == 0) TextureDepth = 1;
		}

		Texture->UpdateResource();
		TextureArray[ArrayIndex] = Texture;
	}

	if (TextureArray.Num() == 0)
		return EErrorCode_FAILED;
	return EErrorCode_OK;
}
