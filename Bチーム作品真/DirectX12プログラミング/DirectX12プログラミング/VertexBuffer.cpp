#include "VertexBuffer.h"
#include <cassert>

VertexBuffer::VertexBuffer(uint32_t vertexStride, uint32_t vertexCount, const void* initialData, ID3D12GraphicsCommandList* commandList)
    : BufferResource()
    , m_stride(vertexStride)
    , m_count(vertexCount)
{
    if (!CreateResource(BufferResourceType::VertexBuffer, (uint64_t)m_stride * m_count, initialData, commandList))
    {
        assert(0);
    }

    m_view.BufferLocation = GetNativeResource()->GetGPUVirtualAddress();
    m_view.SizeInBytes = (UINT)GetBufferSize();
    m_view.StrideInBytes = m_stride;
}
