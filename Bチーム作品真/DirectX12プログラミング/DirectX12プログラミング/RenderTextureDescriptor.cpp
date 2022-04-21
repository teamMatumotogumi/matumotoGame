#include "RenderTextureDescriptor.h"
#include <cassert>


RenderTextureDescriptor::RenderTextureDescriptor()
	: dimension(TextureDimension::Tex2D)
	, width(0)
	, height(0)
	, colorFormat(RenderTextureFormat::Default)
	, graphicsFormat(GraphicsFormat::R32G32B32A32_SFloat)
	, depthBufferBits(0)
	, enableRandomWrite(false) 
{

}


RenderTextureDescriptor::RenderTextureDescriptor(uint32_t width, uint32_t height, RenderTextureFormat colorFormat, uint32_t depthBufferBits)
	: dimension(TextureDimension::Tex2D)
	, width(width)
	, height(height)
	, colorFormat(colorFormat)
	, graphicsFormat(GraphicsFormat::R32G32B32A32_SFloat)
	, depthBufferBits(depthBufferBits)
	, enableRandomWrite(false)
{
	if (!width || !height)
	{
		assert(0);
	}
}
