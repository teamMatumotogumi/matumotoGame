#include "BufferResource.h"
#include "GraphicsEngine.h"
#include "FrameResources.h"
#include <cassert>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// リソースバッファのメモリ種別と書き込み方法に関する詳細情報
//
// D3D12_HEAP_PROPERTIES構造体
// 
//    Type
//       ・D3D12_HEAP_TYPE_DEFAULT   : CPUによる読み書き不可。GPUにとって最適なメモリ上に作成されるためデータ転送速度は最速。
//       ・D3D12_HEAP_TYPE_UPLOAD    : ID3D12Resource::Map()でCPUによる読み書きが可能。ただし、D3D12_HEAP_TYPE_DEFAULT にデータ転送速度で劣る可能性がある。
//       ・D3D12_HEAP_TYPE_CUSTOM    : 配置場所や書き込みモードを指定することができる。 (下記の MemoryPoolPreference が有効になる)
//       ・D3D12_HEAP_TYPE_READBACK  : 
//
//    MemoryPoolPreference
//       ・D3D12_MEMORY_POOL_UNKNOWN : 上記の Type が D3D12_HEAP_TYPE_CUSTOM 以外の場合はこれを指定する必要がある。
//       ・D3D12_MEMORY_POOL_L0      : リソースをシステムメモリ上(SYSMEM)に作成して欲しい。
//       ・D3D12_MEMORY_POOL_L1      : リソースをビデオメモリ上(VRAM)に作成して欲しい。
//
//    CPUPageProperty
//       ・D3D12_CPU_PAGE_PROPERTY_UNKNOWN       : 上記の Type が D3D12_HEAP_TYPE_CUSTOM 以外の場合はこれを指定する必要がある。
//       ・D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE : CPUによる書き込みは不可。
//       ・D3D12_CPU_PAGE_PROPERTY_WRITE_BACK    : CPUによる書き込み時にライトバック方式を採用する。
//       ・D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE : CPUによる書き込み時にライトコンバイン方式を採用する。
//      
//    CreationNodeMask
//         マルチアダプタの場合、このリソースを作成するアダプタを複数選択できる。
//         単一アダプタの場合は 0 または 1 のみ指定可能。(0を指定した場合は、内部で 1 に変換される)
//      
//    VisibleNodeMask
//         マルチアダプタの場合、このリソースを参照できるアダプタを複数選択できる。
//         通常は CreationNodeMask と同じビットセットでよいが、
//         それ以外のアダプタからもアクセスしたい場合はビットを追加することが出来る(…が動作が遅くなる可能性があるので非推奨)。
//         単一アダプタの場合は 0 または 1 のみ指定可能。(0を指定した場合は、内部で 1 に変換される)
//
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------


BufferResource::BufferResource()
    : m_type(BufferResourceType::Unspecified)
    , m_byteWidth(0)
{
}


BufferResource::~BufferResource()
{
    if (m_uploadHeapBuffer)
        m_uploadHeapBuffer->Release();

    if (m_defaultHeapBuffer != m_uploadHeapBuffer)
    {
        if (m_defaultHeapBuffer)
            m_defaultHeapBuffer->Release();
    }
}


ID3D12Resource* BufferResource::CreateUploadHeapBuffer(BufferResourceType bufferResourceType, uint64_t byteWidth, const void* initialData)
{
    // ヒープ詳細情報 (ここで指定したメモリ上にリソースが作成される)
    D3D12_HEAP_PROPERTIES heapProp;
    memset(&heapProp, 0, sizeof(heapProp));
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.CreationNodeMask = 1;
    heapProp.VisibleNodeMask = 1;

    // リソース詳細情報 (ここで指定した種類のリソースが作成される)
    D3D12_RESOURCE_DESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;       // テクスチャ以外のバッファの場合は D3D12_RESOURCE_DIMENSION_BUFFER を指定する。
    desc.Alignment = 0;                                     // アラインメント(単位はバイト)
    desc.Width = byteWidth;                                 // リソースのサイズ(単位はバイト)
    desc.Height = 1;                                        // リソースが2Dまたは3Dテクスチャの場合の「高さ」。(バッファの場合は必ず 1 を指定する)
    desc.DepthOrArraySize = 1;                              // リソースがテクスチャの場合の「深さ」または「配列の要素数」。(バッファの場合は必ず 1 を指定する)
    desc.MipLevels = 1;                                     // リソースがテクスチャの場合の「ミップレベル数」。(バッファの場合は必ず 1 を指定する)
    desc.Format = DXGI_FORMAT_UNKNOWN;                      // リソースがテクスチャの場合の「カラーフォーマット」。(バッファの場合は必ず DXGI_FORMAT_UNKNOWN を指定する)
    desc.SampleDesc.Count = 1;                              // マルチサンプリング数
    desc.SampleDesc.Quality = 0;                            // マルチサンプリング品質
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;           // D3D12_TEXTURE_LAYOUT_UNKNOWN または D3D12_TEXTURE_LAYOUT_ROW_MAJOR を指定する。
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;                  // その他のフラグ

    // リソースの初期状態
    const D3D12_RESOURCE_STATES initialResourceStates
        = D3D12_RESOURCE_STATE_GENERIC_READ                 // GPUから読み取りアクセス可能
        | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE    // ピクセルシェーダーでは未使用
        | D3D12_RESOURCE_STATE_COPY_SOURCE;                 // コピー元として使用


    // アップロードヒープバッファを作成します。
    // 
    //    第1引数 :  [in] ヒープ詳細情報
    //    第2引数 :  [in] ヒープフラグ
    //    第3引数 :  [in] リソース詳細情報
    //    第4引数 :  [in] リソースの初期ステート
    //    第5引数 :  [in] テクスチャをクリアする際の値 (バッファの場合は nullptr でなければならない)
    //    第6引数 :  [in] インターフェースID
    //    第7引数 : [out] リソースを受け取る変数のアドレス
    //     戻り値 :       成否を表すHRESULT型の値
    ID3D12Resource* uploadHeapBuffer;
    if (FAILED(GraphicsEngine::Instance().GetD3D12Device()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc, initialResourceStates, nullptr, IID_ID3D12Resource, (void**)&uploadHeapBuffer)))
    {
        printf("[失敗] アップロードヒープバッファの作成\n");
        assert(0);
    }
    printf("[成功] アップロードヒープバッファの作成 (アドレス: 0x%p)\n", uploadHeapBuffer);


    // 初期データが指定されている場合
    if (initialData)
    {
        // マッピングする範囲を決める (今回はバッファ全体)
        D3D12_RANGE range;
        range.Begin = 0;    // 0バイト目から
        range.End = 0;      // 0バイト目まで

        // アップロードヒープバッファをロックし、CPUからアクセスする為のアドレスを取得する。
        void* writePointer;
        if (FAILED(uploadHeapBuffer->Map(0, &range, &writePointer)))
        {
            assert(0);
        }

        // 初期データをアップロードヒープバッファにコピーする
        memcpy(writePointer, initialData, (uint32_t)desc.Width);

        // アップロードヒープバッファをアンロックする
        uploadHeapBuffer->Unmap(0, nullptr);
    }

    return uploadHeapBuffer;
}


ID3D12Resource* BufferResource::CreateDefaultHeapBuffer(BufferResourceType bufferResourceType, uint64_t byteWidth)
{
    // ヒープ詳細情報 (ここで指定したメモリ上にリソースが作成される)
    D3D12_HEAP_PROPERTIES heapProp;
    memset(&heapProp, 0, sizeof(heapProp));
    heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.CreationNodeMask = 1;
    heapProp.VisibleNodeMask = 1;

    // リソース詳細情報 (ここで指定した種類のリソースが作成される)
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

    // リソースの初期状態
    const D3D12_RESOURCE_STATES initialResourceStates = D3D12_RESOURCE_STATE_COPY_DEST; // コピー先

    // デフォルトヒープバッファを作成します。
    // 
    //    第1引数 :  [in] ヒープ詳細情報
    //    第2引数 :  [in] ヒープフラグ
    //    第3引数 :  [in] リソース詳細情報
    //    第4引数 :  [in] リソースの初期ステート
    //    第5引数 :  [in] テクスチャをクリアする際の値 (バッファの場合は nullptr でなければならない)
    //    第6引数 :  [in] インターフェースID
    //    第7引数 : [out] リソースを受け取る変数のアドレス
    //     戻り値 :       成否を表すHRESULT型の値
    ID3D12Resource* defaultHeapBuffer;
    if (FAILED(GraphicsEngine::Instance().GetD3D12Device()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc, initialResourceStates, nullptr, IID_ID3D12Resource,  (void**)&defaultHeapBuffer)))
    {
        printf("[失敗] デフォルトヒープバッファの作成\n");
        assert(0);
    }
    printf("[成功] デフォルトヒープバッファの作成 (アドレス: 0x%p)\n", defaultHeapBuffer);

    return defaultHeapBuffer;
}


void BufferResource::Upload(BufferResourceType bufferResourceType, ID3D12GraphicsCommandList* commandList)
{
    if (!commandList)
    {
        commandList = GraphicsEngine::Instance().GetCurrentFrameResources()->GetCommandList();
    }

    // リソースバリアで適切なステートに遷移させる
    D3D12_RESOURCE_BARRIER barrier;
    memset(&barrier, 0, sizeof(barrier));
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = m_defaultHeapBuffer;
    barrier.Transition.Subresource = 0;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    
    switch (bufferResourceType)
    {
        case BufferResourceType::ConstantBuffer:
        case BufferResourceType::VertexBuffer:
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
            break;

        case BufferResourceType::IndexBuffer:
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
            break;
    }

    commandList->CopyResource(m_defaultHeapBuffer, m_uploadHeapBuffer);
    commandList->ResourceBarrier(1, &barrier);
}



bool BufferResource::CreateResource(BufferResourceType bufferResourceType, uint64_t byteWidth, const void* initialData, ID3D12GraphicsCommandList* commandList)
{
    m_type = bufferResourceType;
    m_byteWidth = byteWidth;

    switch (bufferResourceType)
    {
        // 定数バッファはMap前提なのでアップロードヒープバッファのみ作成
        case BufferResourceType::ConstantBuffer:
            m_uploadHeapBuffer = CreateUploadHeapBuffer(bufferResourceType, m_byteWidth, initialData);
            m_defaultHeapBuffer = m_uploadHeapBuffer;
            break;


        case BufferResourceType::VertexBuffer:
        case BufferResourceType::IndexBuffer:
            m_uploadHeapBuffer = CreateUploadHeapBuffer(bufferResourceType, m_byteWidth, initialData);
            m_defaultHeapBuffer = CreateDefaultHeapBuffer(bufferResourceType, m_byteWidth);
            if (initialData)
            {
                Upload(bufferResourceType, commandList);
            }
            break;
    }

    return true;
}


void* BufferResource::Map(const MapRange* mapRange)
{
    // どのサブリソースのどの範囲をマップするか？
    uint32_t subresource;
    D3D12_RANGE d3d12Range;

    if (mapRange)
    {
        subresource = mapRange->subresource;
        d3d12Range.Begin = mapRange->begin;
        d3d12Range.End = mapRange->end;
    }
    else
    {
        subresource = 0;
        d3d12Range.Begin = 0;
        d3d12Range.End = 0;
    }

    // アップロードヒープバッファをマップし、CPUからアクセスする為のアドレスを取得する。
    void* mappedPointer;
    if (FAILED(m_uploadHeapBuffer->Map(subresource, &d3d12Range, &mappedPointer)))
    {
        assert(0);
        return nullptr;
    }

    return mappedPointer;
}


void BufferResource::Unmap(const MapRange* mapRange, ID3D12GraphicsCommandList* commandList)
{
    if (mapRange)
    {
        D3D12_RANGE d3d12Range;
        d3d12Range.Begin = mapRange->begin;
        d3d12Range.End = mapRange->end;
        m_uploadHeapBuffer->Unmap(mapRange->subresource, &d3d12Range);
    }
    else
    {
        m_uploadHeapBuffer->Unmap(0, nullptr);
    }

    if (m_defaultHeapBuffer != m_uploadHeapBuffer)
    {
        if (!commandList)
        {
            commandList = GraphicsEngine::Instance().GetCurrentFrameResources()->GetCommandList();
        }

        D3D12_RESOURCE_BARRIER barrier;
        memset(&barrier, 0, sizeof(barrier));
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_defaultHeapBuffer;
        barrier.Transition.Subresource = 0;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        commandList->ResourceBarrier(1, &barrier);

        Upload(m_type, commandList);
    }
}


