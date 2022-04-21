#include "Precompiled.h"
#include "SpriteRendererBatch.h"


// スプライトの描画に使用される頂点を表す構造体
struct SpriteRendererBatch::SpriteVertex
{
    DirectX::XMFLOAT3 position;
    Color color;
    DirectX::XMFLOAT2 texcoord;
};


// 頂点シェーダーで使用される定数バッファを表す構造体
struct SpriteRendererBatch::ConstantBufferForSprite
{
    DirectX::XMFLOAT4X4 worldMatrix;
    Color color;
};


// 頂点シェーダーで使用される定数バッファを表す構造体
struct SpriteRendererBatch::ConstantBufferForCamera
{
    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projMatrix;
};


// 静的メンバ変数の実体を宣言
SpriteRendererBatch::Resources* SpriteRendererBatch::s_resources = nullptr;


void SpriteRendererBatch::CreateCommandListBundle()
{
    ID3D12Device* d3d12Device = GraphicsEngine::Instance().GetD3D12Device();

    // コマンドアロケーターの作成
    ID3D12CommandAllocator* d3d12CommandAllocator;
    if (FAILED(d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_ID3D12CommandAllocator, (void**)&d3d12CommandAllocator)))
    {
        printf("[失敗] スプライトバッチ用コマンドアロケーターの作成\n");
    }
    printf("[成功] スプライトバッチ用コマンドアロケーターの作成 (アドレス: 0x%p)\n", d3d12CommandAllocator);


    // コマンドリストバンドルの作成
    ID3D12GraphicsCommandList* d3d12GraphicsCommandList;
    if (FAILED(d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, d3d12CommandAllocator, nullptr, IID_ID3D12GraphicsCommandList, (void**)&d3d12GraphicsCommandList)))
    {
        printf("[失敗] スプライトバッチ用コマンドリストバンドルの作成\n");
    }
    printf("[成功] スプライトバッチ用コマンドリストバンドルの作成 (アドレス: 0x%p)\n", d3d12GraphicsCommandList);
}


void SpriteRendererBatch::CreateDescriptorHeap()
{
}


void SpriteRendererBatch::LoadAssets()
{
    assert(!s_resources);
    s_resources = new Resources();
    s_resources->vertexBufer = new VertexBuffer(sizeof(SpriteVertex), 1024);                                    // 頂点バッファ
    s_resources->vertexShader = new ShaderBytecode(L"Assets/Shader/SpriteRendererVS.hlsl", "vs_5_1", "main");   // 頂点シェーダー
    s_resources->pixelShader = new ShaderBytecode(L"Assets/Shader/SpriteRendererPS.hlsl", "ps_5_1", "main");    // ピクセルシェーダー
    s_resources->constantBufferForSprite = new ConstantBuffer(sizeof(ConstantBufferForSprite) * 180);           // 定数バッファ
    s_resources->constantBufferForCamera = new ConstantBuffer(sizeof(ConstantBufferForCamera) * 1);             // 定数バッファ

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ルートシグネチャの作成
    //---------------------------------------------------------------------------------------------------------------------------------------------
    static const DescriptorRange descriptorRange[] =
    {
        DescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND),
    };

    RootSignatureBuilder rootSignatureBuilder;
    rootSignatureBuilder.Begin();
    rootSignatureBuilder.AddRootParameter(RootParameterCBV(0, 0, D3D12_SHADER_VISIBILITY_VERTEX));
    rootSignatureBuilder.AddRootParameter(RootParameterCBV(1, 0, D3D12_SHADER_VISIBILITY_VERTEX));
    rootSignatureBuilder.AddRootParameter(RootParameterCBV(2, 0, D3D12_SHADER_VISIBILITY_VERTEX));
    rootSignatureBuilder.AddRootParameter(RootParameterDescriptorTable(_countof(descriptorRange), descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL));
    rootSignatureBuilder.AddStaticSampler(StaticSampler(0));
    rootSignatureBuilder.End(&s_resources->rootSignature);

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 入力レイアウト (英: Input Layout)
    static const D3D12_INPUT_ELEMENT_DESC InputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "VCOLOR",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // グラフィックスパイプラインステート (英: Graphics Pipeline State)
    //---------------------------------------------------------------------------------------------------------------------------------------------
    PipelineStateBuilder pipelineStateBuilder;
    pipelineStateBuilder.Begin(false);
    pipelineStateBuilder.SetRootSignature(s_resources->rootSignature);
    pipelineStateBuilder.IASetInputElementDescs(_countof(InputElementDescs), InputElementDescs);
    pipelineStateBuilder.VSSetShader(s_resources->vertexShader);
    pipelineStateBuilder.PSSetShader(s_resources->pixelShader);
    pipelineStateBuilder.BSSetAlphaToCoverageEnable(true);
    pipelineStateBuilder.BSSetRenderTargetBlend(0, RenderTargetBlend::NonPremultiplied);
    pipelineStateBuilder.OMSetNumRenderTargets(1);
    pipelineStateBuilder.OMSetRenderTargetFormat(0, RenderTargetFormat::R8G8B8A8_UNorm);
    pipelineStateBuilder.OMSetDepthStencilFormat(DepthStencilFormat::Depth32);
    pipelineStateBuilder.End(&s_resources->pipelineState);

    CreateCommandListBundle();
    CreateDescriptorHeap();


    // CBV/SRV用ディスクリプタヒープの作成
    // シェーダーリソースビュー(SRV)の作成 
}


void SpriteRendererBatch::SortOpaqueGeometry()
{

}


void SpriteRendererBatch::SortTransparentGeometry()
{

}


void SpriteRendererBatch::RenderOpaqueGeometry()
{
    for (SpriteRenderer* renderer : s_resources->opaqueGeometries)
    {
    }
}


void SpriteRendererBatch::RenderTransparentGeometry()
{
    for (SpriteRenderer* renderer : s_resources->transparentGeometries)
    {

    }
}


void SpriteRendererBatch::Render()
{










    // 不透過スプライトの描画
    if (!s_resources->opaqueGeometries.empty())
    {
        SortOpaqueGeometry();
        RenderOpaqueGeometry();
    }

    // 透過スプライトの描画
    if (!s_resources->transparentGeometries.empty())
    {
        SortTransparentGeometry();
        RenderTransparentGeometry();
    }
}


void SpriteRendererBatch::Push(SpriteRenderer* spriteRenderer)
{
    const Material* material = spriteRenderer->GetMaterial();

    // 不透過
    if (true /*material->GetRenderingMode() == RenderingMode::Opaque*/)
    {
        s_resources->opaqueGeometries.push_back(spriteRenderer);
    }
    else
    {
        s_resources->transparentGeometries.push_back(spriteRenderer);
    }
}


void SpriteRendererBatch::UnloadAssets()
{





}


