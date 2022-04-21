#pragma once
#include "Texture.h"
#include "GraphicsFormat.h"


enum class RenderTextureFormat
{
	Default,		// プラットフォームのデフォルト形式
	ARGB32,			// 4チャンネルARGB形式。チャンネルあたり8ビット符号付き整数のテクスチャ形式
	Depth,			// 深度をレンダリング可能な32ビット浮動小数点数のテクスチャ形式
	ARGBHalf,		// 4チャンネルARGB形式。チャンネルあたり16ビット浮動小数点数のテクスチャ形式
	ARGBFloat,		// 4チャンネルARGB形式。チャンネルあたり32ビット浮動小数点数のテクスチャ形式
	ARGBInt,		// 4チャンネルARGB形式。チャンネルあたり32ビット符号付き整数のテクスチャ形式
	R8,				// 1チャンネルR形式。  8ビット符号付き整数のテクスチャ形式
	R16,			// 1チャンネルR形式。 16ビット符号付き整数のテクスチャ形式
};


struct RenderTextureDescriptor
{
public:
	TextureDimension dimension;
	uint32_t width;
	uint32_t height;
	RenderTextureFormat colorFormat;
	GraphicsFormat graphicsFormat;
	uint32_t depthBufferBits;
	bool enableRandomWrite;

public:
	// コンストラクタ
	RenderTextureDescriptor();

	// コンストラクタ
	RenderTextureDescriptor(uint32_t width, uint32_t height, RenderTextureFormat colorFormat = RenderTextureFormat::ARGBFloat, uint32_t depthBufferBits = 32);
};


