#include "Texture2D.h"
#include "GraphicsEngine.h"
#include "FrameResources.h"
#include "./External/Include/DirectXTex/DirectXTex.h"


static void MemcpySubresource(const D3D12_MEMCPY_DEST* destination, const D3D12_SUBRESOURCE_DATA* source, size_t rowSizeInBytes, uint32_t numRows, uint32_t numSlices)
{
    for (uint32_t z = 0; z < numSlices; ++z)
    {
        auto destSlice = static_cast<uint8_t*>(destination->pData) + destination->SlicePitch * z;
        auto srcSlice = static_cast<const uint8_t*>(source->pData) + source->SlicePitch * z;

        for (uint32_t y = 0; y < numRows; ++y)
        {
            memcpy(destSlice + destination->RowPitch * y, srcSlice + source->RowPitch * y, rowSizeInBytes);
        }
    }
}


static uint64_t UpdateSubresources(
    ID3D12GraphicsCommandList* commandList,
    ID3D12Resource* destResource,
    ID3D12Resource* intermediateResource,
    uint32_t firstSubresource,
    uint32_t numSubresources,
    uint64_t requiredSize,
    const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* layouts,
    const uint32_t* numRows,
    const uint64_t* rowSizesInBytes,
    const D3D12_SUBRESOURCE_DATA* sourceData) noexcept
{
    D3D12_RESOURCE_DESC intermediateDesc = intermediateResource->GetDesc();
    if (intermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER)
    {
        printf("[失敗] UpdateSubresources() : 指定された中間バッファは D3D12_RESOURCE_DIMENSION_BUFFER で作成されてません\n");
        return 0;
    }

    if (intermediateDesc.Width < requiredSize + layouts[0].Offset)
    {
        printf("[失敗] UpdateSubresources() : 指定された中間バッファは必要サイズを満たしていません\n");
        return 0;
    }

    D3D12_RESOURCE_DESC destinationDesc = destResource->GetDesc();
    if ((destinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER) && (firstSubresource != 0 || numSubresources != 1))
    {
        printf("[失敗] UpdateSubresources() : 指定された転送先バッファは複数のサブリソースに対応していません\n");
        return 0;
    }


    uint8_t* mappedPointer;
    if (FAILED(intermediateResource->Map(0, nullptr, (void**)&mappedPointer)))
    {
        printf("[失敗] UpdateSubresources() : 指定された中間バッファのマッピングに失敗しました\n");
        return 0;
    }

    for (uint32_t i = 0; i < numSubresources; ++i)
    {
        if (rowSizesInBytes[i] > SIZE_T(-1))
            return 0;

        D3D12_MEMCPY_DEST memcpyDest;
        memcpyDest.pData = mappedPointer + layouts[i].Offset;
        memcpyDest.RowPitch = layouts[i].Footprint.RowPitch;
        memcpyDest.SlicePitch = SIZE_T(layouts[i].Footprint.RowPitch) * SIZE_T(numRows[i]);
        MemcpySubresource(&memcpyDest, &sourceData[i], static_cast<SIZE_T>(rowSizesInBytes[i]), numRows[i], layouts[i].Footprint.Depth);
    }
    intermediateResource->Unmap(0, nullptr);

    if (destinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
    {
        commandList->CopyBufferRegion(destResource, 0, intermediateResource, layouts[0].Offset, layouts[0].Footprint.Width);
    }
    else
    {
        for (uint32_t i = 0; i < numSubresources; ++i)
        {
            // コピー先
            D3D12_TEXTURE_COPY_LOCATION destCopy;
            destCopy.pResource = destResource;
            destCopy.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            destCopy.SubresourceIndex = i + firstSubresource;

            // コピー元
            D3D12_TEXTURE_COPY_LOCATION sourceCopy;
            sourceCopy.pResource = intermediateResource;
            sourceCopy.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            sourceCopy.PlacedFootprint = layouts[i];

            // テクスチャ全体をコピー
            commandList->CopyTextureRegion(&destCopy, 0, 0, 0, &sourceCopy, nullptr);
        }
    }

    return requiredSize;
}


Texture2D::Texture2D()
    : m_format(TextureFormat::RGBA32)
    , m_nativeTexture(nullptr)
    , m_descriptorHeap(nullptr)
{

}


Texture2D::~Texture2D()
{
    if (m_descriptorHeap)
        m_descriptorHeap->Release();

    if (m_nativeTexture)
        m_nativeTexture->Release();
}


ID3D12Resource* Texture2D::CreateIntermediateBuffer(ID3D12Device* d3d12Device, uint64_t byteWidth)
{
    // これから作成するリソースのメモリ種別と書き込み方法に関する詳細情報
    D3D12_HEAP_PROPERTIES heapProp;
    memset(&heapProp, 0, sizeof(heapProp));
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.CreationNodeMask = 1;
    heapProp.VisibleNodeMask = 1;

    // これから作成するリソースの詳細情報
    D3D12_RESOURCE_DESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;   // テクスチャ以外のバッファの場合は D3D12_RESOURCE_DIMENSION_BUFFER を指定する。
    desc.Alignment = 0;                                 // アラインメント(単位はバイト)
    desc.Width = byteWidth;                             // リソースのサイズ(単位はバイト)
    desc.Height = 1;                                    // リソースが2Dまたは3Dテクスチャの場合の「高さ」。(バッファの場合は必ず 1 を指定する)
    desc.DepthOrArraySize = 1;                          // リソースがテクスチャの場合の「深さ」または「配列の要素数」。(バッファの場合は必ず 1 を指定する)
    desc.MipLevels = 1;                                 // リソースがテクスチャの場合の「ミップレベル数」。(バッファの場合は必ず 1 を指定する)
    desc.Format = DXGI_FORMAT_UNKNOWN;                  // リソースがテクスチャの場合の「カラーフォーマット」。(バッファの場合は必ず DXGI_FORMAT_UNKNOWN を指定する)
    desc.SampleDesc.Count = 1;                          // マルチサンプリング数
    desc.SampleDesc.Quality = 0;                        // マルチサンプリング品質
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;       // D3D12_TEXTURE_LAYOUT_UNKNOWN または D3D12_TEXTURE_LAYOUT_ROW_MAJOR を指定する。
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;              // その他のフラグ

    ID3D12Resource* intermediate;
    if (FAILED(d3d12Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_ID3D12Resource, (void**)&intermediate)))
    {
        assert(0);
        return nullptr;
    }

    return intermediate;
}


Texture2D* Texture2D::FromFile(const wchar_t* textureFilePath, ID3D12GraphicsCommandList* commandList)
{
    // 画像ファイルフォーマットごとにロードを試みる
    DirectX::TexMetadata texMetadata;
    DirectX::ScratchImage scratchImage;

    // WIC (Windows Imaging Componentの略)
    // Windowsで一般的に用いられている画像フォーマット(.bmp  .png  .jpg  .gif  .tiff)
    if (FAILED(DirectX::LoadFromWICFile(textureFilePath, DirectX::WIC_FLAGS_IGNORE_SRGB, &texMetadata, scratchImage)))
    {
        // DirectXの独自形式ファイル (.dds)
        if (FAILED(DirectX::LoadFromDDSFile(textureFilePath, DirectX::DDS_FLAGS_FORCE_RGB, &texMetadata, scratchImage)))
        {
            // TGA形式ファイル (.tga)
            if (FAILED(DirectX::LoadFromTGAFile(textureFilePath, DirectX::TGA_FLAGS_NONE, &texMetadata, scratchImage)))
            {
                // HDR形式ファイル (.hdr)
                if (FAILED(DirectX::LoadFromHDRFile(textureFilePath, &texMetadata, scratchImage)))
                {
                    assert(0);
                    return nullptr;
                }
            }
        }
    }

    // Direct3D12デバイスを取得する
    ID3D12Device* d3d12Device = GraphicsEngine::Instance().GetD3D12Device();

    // 対象となる画像に合致するテクスチャリソースを作成する
    ID3D12Resource* d3d12Resource;
    if (FAILED(DirectX::CreateTextureEx(d3d12Device, texMetadata, D3D12_RESOURCE_FLAG_NONE, false, &d3d12Resource)))
    {
        assert(0);
    }

    // 全サブリソース情報を取得する
    std::vector<D3D12_SUBRESOURCE_DATA> subresources;
    if (FAILED(DirectX::PrepareUpload(d3d12Device, scratchImage.GetImages(), scratchImage.GetImageCount(), texMetadata, subresources)))
    {
        assert(0);
    }

    const D3D12_RESOURCE_DESC destResourceDesc = d3d12Resource->GetDesc();

    static constexpr UINT MaxSubresources = 128;

    // 占有領域情報(フットプリント)を取得
    UINT numRows[MaxSubresources];
    UINT64 rowSizeInBytes[MaxSubresources];
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts[MaxSubresources];
    UINT64 requiredIntermediateBufferSize;
    d3d12Device->GetCopyableFootprints(&destResourceDesc, 0, (UINT)subresources.size(), 0, layouts, numRows, rowSizeInBytes, &requiredIntermediateBufferSize);

    if (!commandList)
    {
        commandList = GraphicsEngine::Instance().GetCurrentFrameResources()->GetCommandList();
    }

    // 中間バッファの作成
    ID3D12Resource* intermediateBuffer = CreateIntermediateBuffer(d3d12Device, requiredIntermediateBufferSize);
    UpdateSubresources(commandList, d3d12Resource, intermediateBuffer, 0, (uint32_t)subresources.size(), requiredIntermediateBufferSize, layouts, numRows, rowSizeInBytes, subresources.data());

    D3D12_RESOURCE_BARRIER barrier;
    memset(&barrier, 0, sizeof(barrier));
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = d3d12Resource;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    commandList->ResourceBarrier(1, &barrier);

    // シェーダーリソースビュー
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
    memset(&srvDesc, 0, sizeof(srvDesc));
    srvDesc.Format = texMetadata.format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    switch (texMetadata.dimension)
    {
        case DirectX::TEX_DIMENSION_TEXTURE1D:
            if (texMetadata.arraySize > 1)
            {
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
                srvDesc.Texture1DArray.MostDetailedMip = 0;
                srvDesc.Texture1DArray.MipLevels = (UINT)texMetadata.mipLevels;
                srvDesc.Texture1DArray.ResourceMinLODClamp = 0.0f;
                srvDesc.Texture1DArray.FirstArraySlice = 0;
                srvDesc.Texture1DArray.ArraySize = (UINT)texMetadata.arraySize;
            }
            else
            {
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
                srvDesc.Texture1D.MostDetailedMip = 0;
                srvDesc.Texture1D.MipLevels = (UINT)texMetadata.mipLevels;
                srvDesc.Texture1D.ResourceMinLODClamp = 0.0f;
            }
            break;

        case DirectX::TEX_DIMENSION_TEXTURE2D:
            if (texMetadata.arraySize > 1)
            {
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
                srvDesc.Texture2DArray.MostDetailedMip = 0;
                srvDesc.Texture2DArray.MipLevels = (UINT)texMetadata.mipLevels;
                srvDesc.Texture2DArray.PlaneSlice = 0;
                srvDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
                srvDesc.Texture2DArray.FirstArraySlice = 0;
                srvDesc.Texture2DArray.ArraySize = (UINT)texMetadata.arraySize;
            }
            else
            {
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MostDetailedMip = 0;
                srvDesc.Texture2D.MipLevels = (UINT)texMetadata.mipLevels;
                srvDesc.Texture2D.PlaneSlice = 0;
                srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
            }
            break;

        case DirectX::TEX_DIMENSION_TEXTURE3D:
            if (texMetadata.arraySize > 1)
            {
                assert(0);
            }
            else
            {
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
                srvDesc.Texture3D.MostDetailedMip = 0;
                srvDesc.Texture3D.MipLevels = (UINT)texMetadata.mipLevels;
                srvDesc.Texture3D.ResourceMinLODClamp = 0.0f;
            }
            break;
    }

    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc;
    memset(&descHeapDesc, 0, sizeof(descHeapDesc));
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.NumDescriptors = 1;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NodeMask = 0;

    ID3D12DescriptorHeap* descriptorHeap;
    if (FAILED(d3d12Device->CreateDescriptorHeap(&descHeapDesc, IID_ID3D12DescriptorHeap, (void**)&descriptorHeap)))
    {
        assert(0);
    }

    d3d12Device->CreateShaderResourceView(d3d12Resource, &srvDesc, descriptorHeap->GetCPUDescriptorHandleForHeapStart());

    Texture2D* product = new Texture2D();
    product->m_dimension = TextureDimension::Tex2D;
    product->m_width = (int)destResourceDesc.Width;
    product->m_height = (int)destResourceDesc.Height;
    product->m_format = TextureFormat::RGBA32;
    product->m_filterMode = FilterMode::Point;
    product->m_anisoLevel = 0;
    product->m_isReadable = false;
    product->m_mipMapBias = 0.0f;
    product->m_nativeTexture = d3d12Resource;
    product->m_descriptorHeap = descriptorHeap;
    return product;
}


