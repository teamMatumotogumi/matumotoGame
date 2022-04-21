#include "RenderTexture.h"
#include "GraphicsEngine.h"
#include <d3d12.h>
#include <cassert>

RenderTexture* RenderTexture::s_active = nullptr;


void RenderTexture::Construct()
{
	m_readWrite = RenderTextureReadWrite::Default;
	m_colorBuffer = nullptr;
	m_depthBuffer = nullptr;
	m_isCreated = false;
}


RenderTexture::RenderTexture(uint32_t width, uint32_t height, uint32_t depth, RenderTextureFormat format, RenderTextureReadWrite readWrite)
{
	Construct();
	m_descriptor.dimension = TextureDimension::Tex2D;
	m_descriptor.width = width;
	m_descriptor.height = height;
	m_descriptor.colorFormat = format;
	m_descriptor.depthBufferBits = depth;
}


RenderTexture::RenderTexture(const RenderTextureDescriptor& descriptor)
{
	Construct();
	m_descriptor = descriptor;
}


RenderTexture::RenderTexture(const RenderTexture* textureToCopy)
{
	assert(textureToCopy);

	Construct();
	m_descriptor = textureToCopy->m_descriptor;
}


RenderTexture::~RenderTexture()
{

}


bool RenderTexture::Create()
{
	if (!m_isCreated)
	{
		// サイズが不正
		if (m_descriptor.width <= 0 || m_descriptor.height <= 0)
		{
			assert(0);
		}

		m_colorBuffer = CreateColorBuffer();
		m_depthBuffer = CreateDepthBuffer();
		m_isCreated = true;
	}

	return m_isCreated;
}



RenderBuffer* RenderTexture::CreateColorBuffer()
{
	// TODO: カラーバッファの作成

	D3D12_HEAP_PROPERTIES heapProps;
	memset(&heapProps, 0, sizeof(heapProps));
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.CreationNodeMask = 0;
	heapProps.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resourceDesc;
	memset(&resourceDesc, 0, sizeof(resourceDesc));
	resourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	resourceDesc.Width = m_descriptor.width;
	resourceDesc.Height = m_descriptor.height;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Alignment = 0;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	ID3D12Device* d3d12Device = GraphicsEngine::Instance().GetD3D12Device();
	//d3d12Device->CreateCommittedResource(&heapProps, );


	return nullptr;
}


RenderBuffer* RenderTexture::CreateDepthBuffer()
{
	// TODO: 深度ステンシルバッファの作成
	switch (m_descriptor.depthBufferBits)
	{
	case 0:
		return nullptr;

	case 16:
		break;

	case 24:
		break;

	case 32:
		break;

	default:
		assert(!"Not supported precious of depth/stencil format.");
		break;
	}

	return nullptr;
}


void* RenderTexture::GetNativeDepthBufferPtr() const
{
	return m_depthBuffer ? m_depthBuffer->m_nativeBufferPtr : nullptr;
}


RenderTexture* RenderTexture::GetTemporary(const RenderTextureDescriptor* descriptor)
{
	return nullptr;
}


void RenderTexture::ReleaseTemporary(RenderTexture* temp)
{

}


