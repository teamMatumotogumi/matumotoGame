#include "ConstantBuffer.h"
#include <cassert>

ConstantBuffer::ConstantBuffer(uint32_t byteWidth, const void* initialData, ID3D12GraphicsCommandList* commandList)
    : BufferResource()
{
    // Direct3D12の定数バッファは256の倍数バイトでなければならない
    byteWidth = ((byteWidth + 0xff) & ~0xff);

    if (!CreateResource(BufferResourceType::ConstantBuffer, byteWidth, initialData, commandList))
    {
        assert(0);
    }

    memset(&m_defaultCBV, 0, sizeof(m_defaultCBV));
    m_defaultCBV.SizeInBytes = byteWidth;
    m_defaultCBV.BufferLocation = ((ID3D12Resource*)GetNativeResource())->GetGPUVirtualAddress();
}
