#include "IndexBuffer.h"
#include <cassert>


IndexBuffer::IndexBuffer(IndexFormat indexFormat, uint32_t indexCount, const void* initialData, ID3D12GraphicsCommandList* commandList)
    : BufferResource()
    , m_format(indexFormat)
    , m_stride(0)
    , m_count(indexCount)
{
    switch (m_format)
    {
        case IndexFormat::UInt16: m_stride = 2; break;
        case IndexFormat::UInt32: m_stride = 4; break;
        default: assert(0); break;
    }

    if (!CreateResource(BufferResourceType::IndexBuffer, (uint64_t)m_stride * m_count, initialData, commandList))
    {
        assert(0);
    }

    m_view.BufferLocation = GetNativeResource()->GetGPUVirtualAddress();
    m_view.SizeInBytes = (UINT)GetBufferSize();
    m_view.Format = GetIndexFormatAsDXGI();
}

IndexBuffer::~IndexBuffer()
{
}


DXGI_FORMAT IndexBuffer::GetIndexFormatAsDXGI() const
{
    return (m_format == IndexFormat::UInt16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
}

