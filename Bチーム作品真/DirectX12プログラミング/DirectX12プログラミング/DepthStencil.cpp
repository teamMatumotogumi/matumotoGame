#include "DepthStencil.h"
#include "GraphicsEngine.h"
#include <cassert>


DepthStencil::DepthStencil(uint32_t width, uint32_t height, DepthStencilFormat format)
    : m_width(0)
    , m_height(0)
    , m_format(DepthStencilFormat::Default)
{
    Create(width, height, format);
}


DepthStencil::~DepthStencil()
{
    if (m_depthStencilBuffer)
        m_depthStencilBuffer->Release();

    if (m_descriptorHeap)
        m_descriptorHeap->Release();
}


DXGI_FORMAT DepthStencil::ConvertFrom(DepthStencilFormat format)
{
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //   DXGI_FORMAT_D32_FLOAT_S8X24_UINT
    //   DXGI_FORMAT_D32_FLOAT              // [0.0～1.0] 10の38乗の精度
    //   DXGI_FORMAT_D24_UNORM_S8_UINT      // [0.0～1.0] 0～1766万の精度
    //   DXGI_FORMAT_D16_UNORM              // [0.0～1.0] 0～65535 の精度
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    DXGI_FORMAT result;
    switch (format)
    {
    case DepthStencilFormat::Depth16:           result = DXGI_FORMAT_D16_UNORM; break;
    case DepthStencilFormat::Depth24Stencil8:   result = DXGI_FORMAT_D24_UNORM_S8_UINT; break;
    case DepthStencilFormat::Depth32:           result = DXGI_FORMAT_D32_FLOAT; break;
    default: assert(0); break;
    }
    return result;
}


void DepthStencil::Create(uint64_t width, uint32_t height, DepthStencilFormat format)
{
    ID3D12Device* d3d12Device = GraphicsEngine::Instance().GetD3D12Device();

    m_width = width;
    m_height = height;
    m_format = format;
    m_depthStencilBuffer = CreateDepthStencilBuffer(d3d12Device, m_width, m_height, m_format);
    m_descriptorHeap = CreateDescriptorHeapForDSV(d3d12Device, 1);

    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // 深度ステンシルビュー(DSV)の作成
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //D3D12_DEPTH_STENCIL_VIEW_DESC desc;
    //memset(&desc, 0, sizeof(desc));
    //desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    //desc.Format = dxgiFormat;
    //desc.Texture2D.MipSlice = 0;
    //desc.Flags = D3D12_DSV_FLAG_NONE;

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 深度ステンシルビューの作成
    //
    //  ID3D12Device::CreateDepthStencilView
    //  (
    //      第1引数 : [入力] アクセス対象となるリソース
    //      第2引数 : [入力] アクセスする為の詳細情報 (既定値の場合は nullptr でも可)
    //      第3引数 : [入力] 深度ステンシルビューを作成するディスクリプタへのポインタ
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    d3d12Device->CreateDepthStencilView(m_depthStencilBuffer, nullptr,  m_descriptorHeap->GetCPUDescriptorHandleForHeapStart());
}


ID3D12Resource* DepthStencil::CreateDepthStencilBuffer(ID3D12Device* d3d12Device, uint64_t width, uint32_t height, DepthStencilFormat format)
{
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ヒープ詳細情報
    //---------------------------------------------------------------------------------------------------------------------------------------------
    D3D12_HEAP_PROPERTIES heapProps;
    memset(&heapProps, 0, sizeof(heapProps));
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;						                    // ビデオメモリ上に作成したい
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;                         // どのメモリ上に作成する？ (Type == D3D12_HEAP_TYPE_CUSTOM)時のみ有効
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;                        // CPU側からアクセスする？  (Type == D3D12_HEAP_TYPE_CUSTOM)時のみ有効
    heapProps.CreationNodeMask = 1;                                                     // 単一アダプターの場合は 0 または 1
    heapProps.VisibleNodeMask = 1;                                                      // 単一アダプターの場合は 0 または 1

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 深度ステンシルバッファ作成に関する詳細情報
    //---------------------------------------------------------------------------------------------------------------------------------------------
    D3D12_RESOURCE_DESC depthStencilCreationDesc;
    memset(&depthStencilCreationDesc, 0, sizeof(depthStencilCreationDesc));
    depthStencilCreationDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;            // 2次元のテクスチャデータとして作成 (これ以外だと失敗する？)
    depthStencilCreationDesc.Format = ConvertFrom(format);                              // 深度値とステンシル値のフォーマット
    depthStencilCreationDesc.Width = width;                                             // 深度ステンシルバッファの横幅 (レンダーターゲットと同じか、もしくは、広くなければならない)
    depthStencilCreationDesc.Height = height;                                           // 深度ステンシルバッファの高さ (レンダーターゲットと同じか、もしくは、広くなければならない)
    depthStencilCreationDesc.DepthOrArraySize = 1;                                      // 「奥行数」または「配列要素数」(深度ステンシルバッファの場合は 1)
    depthStencilCreationDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;    // (0 または D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT で 64KB)
    depthStencilCreationDesc.MipLevels = 1;                                             // ミップレベル数 (0の場合は全てのミップレベルが作成される)
    depthStencilCreationDesc.SampleDesc.Count = 1;                                      // マルチサンプリング数   (深度ステンシルでは使用できないので 1)
    depthStencilCreationDesc.SampleDesc.Quality = 0;                                    // マルチサンプリング品質 (深度ステンシルでは使用できないので 0)
    depthStencilCreationDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;                     // 
    depthStencilCreationDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;           // 「深度ステンシルとして使用することを許可する」フラグ

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 深度ステンシルバッファのクリア値情報
    //---------------------------------------------------------------------------------------------------------------------------------------------
    D3D12_CLEAR_VALUE clearValue;
    memset(&clearValue, 0, sizeof(clearValue));
    clearValue.Format = depthStencilCreationDesc.Format;                                // 深度値とステンシル値のフォーマット
    clearValue.DepthStencil.Depth = 1.0f;                                               // 深度値は1.0でクリア
    clearValue.DepthStencil.Stencil = 0;                                                // ステンシル値は0でクリア

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // リソースの作成 (今回は深度ステンシルバッファ)
    //
    //  ID3D12Device::CreateCommittedResource
    //  (
    //      第1引数 : [入力] ヒープ詳細情報
    //      第2引数 : [入力] ヒープに関する各種オプションフラグ (最適化ヒント、エラーチェック等)
    //      第3引数 : [入力] リソース作成に関する詳細情報
    //      第4引数 : [入力] リソースの初期ステート
    //      第5引数 : [入力] リソースのクリア値情報
    //      第6引数 : [入力] インターフェースID
    //      第7引数 : [出力] インターフェースを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    ID3D12Resource* depthStencilBuffer;
    if (FAILED(d3d12Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &depthStencilCreationDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_ID3D12Resource, (void**)&depthStencilBuffer)))
    {
        assert(0);
    }

    return depthStencilBuffer;
}


ID3D12DescriptorHeap* DepthStencil::CreateDescriptorHeapForDSV(ID3D12Device* d3d12Device, UINT numDescriptors)
{
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // ディスクリプタヒープ作成に関する詳細情報
    //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapCreationDesc;
    memset(&descriptorHeapCreationDesc, 0, sizeof(descriptorHeapCreationDesc));
    descriptorHeapCreationDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;       // ディスクリプタの種類 (今回はDSV)
    descriptorHeapCreationDesc.NumDescriptors = numDescriptors;             // ディスクリプタ配列の要素数
    descriptorHeapCreationDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;     // 各種オプションフラグ
    descriptorHeapCreationDesc.NodeMask = 0;                                // 単一アダプターの場合は 0 または 1

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ディスクリプタヒープの作成
    // 
    //  ID3D12Device::CreateDescriptorHeap
    //  (
    //      第1引数 : [入力] ディスクリプタヒープ作成に関する詳細情報
    //      第2引数 : [入力] インターフェースID
    //      第3引数 : [出力] インターフェースを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    ID3D12DescriptorHeap* descriptorHeap;
    if (FAILED(d3d12Device->CreateDescriptorHeap(&descriptorHeapCreationDesc, IID_ID3D12DescriptorHeap, (void**)&descriptorHeap)))
    {
        assert(0);
    }

    return descriptorHeap;
}
