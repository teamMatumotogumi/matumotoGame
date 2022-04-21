#include "Precompiled.h"
#include "PipelineStateBuilder.h"
#include "DepthStencilFormat.h"
#include "GraphicsEngine.h"
#include "ShaderBytecode.h"
#include <d3d12.h>
#include <cassert>


PipelineStateBuilder::PipelineStateBuilder()
    : m_isBegun(false)
{
    ResetToDefault();
}


void PipelineStateBuilder::ResetToDefault()
{
    // 入力アセンブラ (IA)
    m_rootSignature = nullptr;
    m_primitiveTopology = PrimitiveTopology::Triangle;
    m_indexBufferStripCutValue = IndexBufferStripCutValue::Disable;
    m_numInputElementDescs = 0;
    memset(m_inputElementDescs, 0, sizeof(m_inputElementDescs));

    // プログラマブルシェーダー
    m_VS = nullptr; // 頂点シェーダー (VS)
    m_DS = nullptr; // ドメインシェーダー (DS)
    m_HS = nullptr; // ハルシェーダー (HS)
    m_GS = nullptr; // ジオメトリシェーダー (GS)
    m_PS = nullptr; // ピクセルシェーダー (PS)

    // マルチサンプリング
    m_multiSampleCount = 1;
    m_multiSampleQuality = 0;

    // ブレンドステート (BS)
    m_alphaToCoverageEnable = false;
    m_independentBlendEnable = false;
    m_blendSampleMask = UINT32_MAX;
    for (uint32_t i = 0; i < 8; i++)
    {
        m_renderTargetBlend[i] = RenderTargetBlend::Opaque;
    }

    // ラスタライザーステート (RS)
    m_fillMode = FillMode::Solid;
    m_cullMode = CullMode::Back;
    m_frontCounterClockwise = false;
    m_depthBias = 0;
    m_depthBiasClamp = 0.0f;
    m_slopeScaledDepthBias = 0.0f;
    m_depthClipEnable = true;
    m_multisampleEnable = false;
    m_antialiasedLineEnable = false;
    m_forcedSampleCount = 0;
    m_conservativeRasterEnable = false;

    // デプスステンシルステート
    m_depthEnable = true;
    m_depthWriteEnable = true;
    m_depthFunc = ComparisonFunc::Less;
    m_stencilEnable = false;
    m_stencilReadMask = 0xff;
    m_stencilWriteMask = 0xff;
    m_frontFaceStencilFailOp = StencilOperation::Keep;
    m_frontFaceStencilDepthFailOp = StencilOperation::Keep;
    m_frontFaceStencilPassOp = StencilOperation::Keep;
    m_frontFaceStencilFunc = ComparisonFunc::Always;
    m_backFaceStencilFailOp = StencilOperation::Keep;
    m_backFaceStencilDepthFailOp = StencilOperation::Keep;
    m_backFaceStencilPassOp = StencilOperation::Keep;
    m_backFaceStencilFunc = ComparisonFunc::Always;

    // 出力マージャー (OM)
    m_numRenderTargets = 0;
    memset(m_renderTargetFormats, 0, sizeof(m_renderTargetFormats));
    m_depthStencilFormat = DepthStencilFormat::Default;
}


void PipelineStateBuilder::Begin(bool resetToDefault)
{
    assert(!m_isBegun);
    if (resetToDefault)
    {
        ResetToDefault();
    }
    m_isBegun = true;
}


void PipelineStateBuilder::IASetInputElementDescs(uint32_t numInputElementDescs, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs)
{
    memcpy(m_inputElementDescs, inputElementDescs, sizeof(D3D12_INPUT_ELEMENT_DESC) * numInputElementDescs);
    m_numInputElementDescs = numInputElementDescs;
}


static D3D12_BLEND ConvertFrom(BlendFactor factor)
{
    D3D12_BLEND result;
    switch (factor)
    {
    case BlendFactor::Zero:                     result = D3D12_BLEND_ZERO; break;
    case BlendFactor::One:                      result = D3D12_BLEND_ONE; break;
    case BlendFactor::SourceColor:              result = D3D12_BLEND_SRC_COLOR; break;
    case BlendFactor::SourceAlpha:              result = D3D12_BLEND_SRC_ALPHA; break;
    case BlendFactor::SourceAlphaSaturate:      result = D3D12_BLEND_SRC_ALPHA_SAT; break;
    case BlendFactor::Source1Color:             result = D3D12_BLEND_SRC1_COLOR; break;
    case BlendFactor::Source1Alpha:             result = D3D12_BLEND_SRC1_ALPHA; break;
    case BlendFactor::BlendFactor:              result = D3D12_BLEND_BLEND_FACTOR; break;
    case BlendFactor::DestinationColor:         result = D3D12_BLEND_DEST_COLOR; break;
    case BlendFactor::DestinationAlpha:         result = D3D12_BLEND_DEST_ALPHA; break;
    case BlendFactor::InverseSourceColor:       result = D3D12_BLEND_INV_SRC_COLOR; break;
    case BlendFactor::InverseSourceAlpha:       result = D3D12_BLEND_INV_SRC_ALPHA; break;
    case BlendFactor::InverseSource1Color:      result = D3D12_BLEND_INV_SRC1_COLOR; break;
    case BlendFactor::InverseSource1Alpha:      result = D3D12_BLEND_INV_SRC1_ALPHA; break;
    case BlendFactor::InverseBlendFactor:       result = D3D12_BLEND_INV_BLEND_FACTOR; break;
    case BlendFactor::InverseDestinationColor:  result = D3D12_BLEND_INV_DEST_COLOR; break;
    case BlendFactor::InverseDestinationAlpha:  result = D3D12_BLEND_INV_DEST_ALPHA; break;
    default: assert(0); break;
    }
    return result;
}


static D3D12_BLEND_OP ConvertFrom(BlendOperation operation)
{
    D3D12_BLEND_OP result;
    switch (operation)
    {
    case BlendOperation::Add:             result = D3D12_BLEND_OP_ADD; break;
    case BlendOperation::Subtract:        result = D3D12_BLEND_OP_SUBTRACT; break;
    case BlendOperation::ReverseSubtract: result = D3D12_BLEND_OP_REV_SUBTRACT; break;
    case BlendOperation::Min:             result = D3D12_BLEND_OP_MIN; break;
    case BlendOperation::Max:             result = D3D12_BLEND_OP_MAX; break;
    default: assert(0); break;
    }
    return result;
}


static D3D12_LOGIC_OP ConvertFrom(LogicOperation operation)
{
    D3D12_LOGIC_OP result;
    switch (operation)
    {
    case LogicOperation::Clear:         result = D3D12_LOGIC_OP_CLEAR; break;
    case LogicOperation::Set:           result = D3D12_LOGIC_OP_SET; break;
    case LogicOperation::Copy:          result = D3D12_LOGIC_OP_COPY; break;
    case LogicOperation::CopyInverted:  result = D3D12_LOGIC_OP_COPY_INVERTED; break;
    case LogicOperation::Noop:          result = D3D12_LOGIC_OP_NOOP; break;
    case LogicOperation::Invert:        result = D3D12_LOGIC_OP_INVERT; break;
    case LogicOperation::NotAnd:        result = D3D12_LOGIC_OP_NAND; break;
    case LogicOperation::Or:            result = D3D12_LOGIC_OP_OR; break;
    case LogicOperation::Nor:           result = D3D12_LOGIC_OP_NOR; break;
    case LogicOperation::Xor:           result = D3D12_LOGIC_OP_XOR; break;
    case LogicOperation::Equivalence:   result = D3D12_LOGIC_OP_EQUIV; break;
    case LogicOperation::AndReverse:    result = D3D12_LOGIC_OP_AND_REVERSE; break;
    case LogicOperation::AndInverted:   result = D3D12_LOGIC_OP_AND_INVERTED; break;
    case LogicOperation::OrReverse:     result = D3D12_LOGIC_OP_OR_REVERSE; break;
    case LogicOperation::OrInverted:    result = D3D12_LOGIC_OP_OR_INVERTED; break;
    default: assert(0); break;
    }
    return result;
}


static uint32_t ConvertFrom(ColorWriteEnable mask)
{
    uint32_t result = 0;
    if (mask & ColorWriteEnable::Red)   result |= D3D12_COLOR_WRITE_ENABLE_RED;
    if (mask & ColorWriteEnable::Green) result |= D3D12_COLOR_WRITE_ENABLE_GREEN;
    if (mask & ColorWriteEnable::Blue)  result |= D3D12_COLOR_WRITE_ENABLE_BLUE;
    if (mask & ColorWriteEnable::Alpha) result |= D3D12_COLOR_WRITE_ENABLE_ALPHA;
    return result;
}


static D3D12_PRIMITIVE_TOPOLOGY_TYPE ConvertFrom(PrimitiveTopology primitiveTopology)
{
    D3D12_PRIMITIVE_TOPOLOGY_TYPE result;
    switch (primitiveTopology)
    {
    case PrimitiveTopology::Point:    result = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT; break;
    case PrimitiveTopology::Line:     result = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE; break;
    case PrimitiveTopology::Triangle: result = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; break;
    case PrimitiveTopology::Patch:    result = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH; break;
    default: assert(0); break;
    }
    return result;
}


static D3D12_INDEX_BUFFER_STRIP_CUT_VALUE ConvertFrom(IndexBufferStripCutValue indexBufferStripCutValue)
{
    D3D12_INDEX_BUFFER_STRIP_CUT_VALUE result;
    switch (indexBufferStripCutValue)
    {
    case IndexBufferStripCutValue::Disable:   result = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED; break;
    case IndexBufferStripCutValue::UInt16Max: result = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF; break;
    case IndexBufferStripCutValue::UInt32Max: result = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF; break;
    default: assert(0); break;
    }
    return result;
}


static D3D12_FILL_MODE ConvertFrom(FillMode fillMode)
{
    D3D12_FILL_MODE result;
    switch (fillMode)
    {
    case FillMode::Solid:     result = D3D12_FILL_MODE_SOLID; break;
    case FillMode::Wireframe: result = D3D12_FILL_MODE_WIREFRAME; break;
    default: assert(0); break;
    }
    return result;
}


static D3D12_CULL_MODE ConvertFrom(CullMode cullMode)
{
    D3D12_CULL_MODE result;
    switch (cullMode)
    {
    case CullMode::Off:   result = D3D12_CULL_MODE_NONE; break;
    case CullMode::Back:  result = D3D12_CULL_MODE_BACK; break;
    case CullMode::Front: result = D3D12_CULL_MODE_FRONT; break;
    default: assert(0); break;
    }
    return result;
}


static D3D12_CONSERVATIVE_RASTERIZATION_MODE ConvertCoservativeRasterizationFrom(bool conservativeRasterEnable)
{
    return conservativeRasterEnable ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}


static D3D12_DEPTH_WRITE_MASK ConvertDepthWriteMastFrom(bool depthWriteEnable)
{
    return depthWriteEnable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
}


static D3D12_COMPARISON_FUNC ConvertFrom(ComparisonFunc func)
{
    D3D12_COMPARISON_FUNC result;
    switch (func)
    {
    case ComparisonFunc::Never:         result = D3D12_COMPARISON_FUNC_NEVER; break;
    case ComparisonFunc::Less:          result = D3D12_COMPARISON_FUNC_LESS; break;
    case ComparisonFunc::Equal:         result = D3D12_COMPARISON_FUNC_EQUAL; break;
    case ComparisonFunc::LessEqual:     result = D3D12_COMPARISON_FUNC_LESS_EQUAL; break;
    case ComparisonFunc::Greater:       result = D3D12_COMPARISON_FUNC_GREATER; break;
    case ComparisonFunc::NotEqual:      result = D3D12_COMPARISON_FUNC_NOT_EQUAL; break;
    case ComparisonFunc::GreaterEqual:  result = D3D12_COMPARISON_FUNC_GREATER_EQUAL; break;
    case ComparisonFunc::Always:        result = D3D12_COMPARISON_FUNC_ALWAYS; break;
    default: assert(0); break;
    }
    return result;
}


static D3D12_STENCIL_OP ConvertFrom(StencilOperation operation)
{
    D3D12_STENCIL_OP result;
    switch (operation)
    {
    case StencilOperation::Keep:              result = D3D12_STENCIL_OP_KEEP; break;
    case StencilOperation::Zero:              result = D3D12_STENCIL_OP_ZERO; break;
    case StencilOperation::Replace:           result = D3D12_STENCIL_OP_REPLACE; break;
    case StencilOperation::IncrementSaturate: result = D3D12_STENCIL_OP_INCR_SAT; break;
    case StencilOperation::DecrementSaturate: result = D3D12_STENCIL_OP_DECR_SAT; break;
    case StencilOperation::Invert:            result = D3D12_STENCIL_OP_INVERT; break;
    case StencilOperation::Increment:         result = D3D12_STENCIL_OP_INCR; break;
    case StencilOperation::Decrement:         result = D3D12_STENCIL_OP_DECR; break;
    default: assert(0); break;
    }
    return result;
}


static DXGI_FORMAT ConvertFrom(RenderTargetFormat format)
{
    DXGI_FORMAT result;
    switch (format)
    {
    case RenderTargetFormat::R32G32B32A32_Float: result = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
    case RenderTargetFormat::R32G32B32A32_UInt:  result = DXGI_FORMAT_R32G32B32A32_UINT; break;
    case RenderTargetFormat::R32G32B32A32_SInt:  result = DXGI_FORMAT_R32G32B32A32_SINT; break;
    case RenderTargetFormat::R16G16B16A16_Float: result = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
    case RenderTargetFormat::R16G16B16A16_UNorm: result = DXGI_FORMAT_R16G16B16A16_UNORM; break;
    case RenderTargetFormat::R16G16B16A16_UInt:  result = DXGI_FORMAT_R16G16B16A16_UINT; break;
    case RenderTargetFormat::R16G16B16A16_SNorm: result = DXGI_FORMAT_R16G16B16A16_SNORM; break;
    case RenderTargetFormat::R16G16B16A16_SInt:  result = DXGI_FORMAT_R16G16B16A16_SINT; break;
    case RenderTargetFormat::R8G8B8A8_UNorm:     result = DXGI_FORMAT_R8G8B8A8_UNORM; break;
    case RenderTargetFormat::R8G8B8A8_UInt:      result = DXGI_FORMAT_R8G8B8A8_UINT; break;
    case RenderTargetFormat::R8G8B8A8_SNorm:     result = DXGI_FORMAT_R8G8B8A8_SNORM; break;
    case RenderTargetFormat::R8G8B8A8_SInt:      result = DXGI_FORMAT_R8G8B8A8_SINT; break;
    default: assert(0); break;
    }
    return result;
}


static DXGI_FORMAT ConvertFrom(DepthStencilFormat format)
{
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

static D3D12_SHADER_BYTECODE ConvertFrom(ShaderBytecode* shaderBytecode)
{
    D3D12_SHADER_BYTECODE d3d12ShaderBytecode;
    if (shaderBytecode)
    {
        d3d12ShaderBytecode.BytecodeLength = shaderBytecode->GetBufferSize();
        d3d12ShaderBytecode.pShaderBytecode = shaderBytecode->GetBufferPointer();
    }
    else
    {
        d3d12ShaderBytecode.BytecodeLength = 0;
        d3d12ShaderBytecode.pShaderBytecode = nullptr;
    }
    return d3d12ShaderBytecode;
}


void PipelineStateBuilder::End(ID3D12PipelineState** ppD3D12PipelineState)
{
    assert(m_isBegun);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    memset(&psoDesc, 0, sizeof(psoDesc));

    psoDesc.pRootSignature = m_rootSignature;

    // 入力アセンブラ (IA)
    psoDesc.InputLayout.NumElements = m_numInputElementDescs;
    psoDesc.InputLayout.pInputElementDescs = m_inputElementDescs;
    psoDesc.PrimitiveTopologyType = ConvertFrom(m_primitiveTopology);
    psoDesc.IBStripCutValue = ConvertFrom(m_indexBufferStripCutValue);

    // 各種シェーダー
    psoDesc.VS = ConvertFrom(m_VS);
    psoDesc.DS = ConvertFrom(m_DS);
    psoDesc.HS = ConvertFrom(m_HS);
    psoDesc.GS = ConvertFrom(m_GS);
    psoDesc.PS = ConvertFrom(m_PS);

    // ブレンドステート (BS)
    psoDesc.BlendState.AlphaToCoverageEnable = m_alphaToCoverageEnable;
    psoDesc.BlendState.IndependentBlendEnable = m_independentBlendEnable;
    for (uint32_t i = 0; i < 8; i++)
    {
        psoDesc.BlendState.RenderTarget[i].BlendEnable = m_renderTargetBlend[i].BlendEnable;
        psoDesc.BlendState.RenderTarget[i].LogicOpEnable = m_renderTargetBlend[i].LogicOpEnable;
        psoDesc.BlendState.RenderTarget[i].SrcBlend = ConvertFrom(m_renderTargetBlend[i].SrcBlend);
        psoDesc.BlendState.RenderTarget[i].SrcBlendAlpha = ConvertFrom(m_renderTargetBlend[i].SrcBlendAlpha);
        psoDesc.BlendState.RenderTarget[i].DestBlend = ConvertFrom(m_renderTargetBlend[i].DestBlend);
        psoDesc.BlendState.RenderTarget[i].DestBlendAlpha = ConvertFrom(m_renderTargetBlend[i].DestBlendAlpha);
        psoDesc.BlendState.RenderTarget[i].BlendOp = ConvertFrom(m_renderTargetBlend[i].BlendOp);
        psoDesc.BlendState.RenderTarget[i].BlendOpAlpha = ConvertFrom(m_renderTargetBlend[i].BlendOpAlpha);
        psoDesc.BlendState.RenderTarget[i].LogicOp = ConvertFrom(m_renderTargetBlend[i].LogicOp);
        psoDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = ConvertFrom(m_renderTargetBlend[i].RenderTargetWriteMask);
    }

    // ラスタライザーステート (RS)
    psoDesc.RasterizerState.FillMode = ConvertFrom(m_fillMode);
    psoDesc.RasterizerState.CullMode = ConvertFrom(m_cullMode);
    psoDesc.RasterizerState.FrontCounterClockwise = m_frontCounterClockwise;
    psoDesc.RasterizerState.DepthBias = m_depthBias;
    psoDesc.RasterizerState.DepthBiasClamp = m_depthBiasClamp;
    psoDesc.RasterizerState.SlopeScaledDepthBias = m_slopeScaledDepthBias;
    psoDesc.RasterizerState.DepthClipEnable = m_depthClipEnable;
    psoDesc.RasterizerState.MultisampleEnable = m_multisampleEnable;
    psoDesc.RasterizerState.AntialiasedLineEnable = m_antialiasedLineEnable;
    psoDesc.RasterizerState.ForcedSampleCount = m_forcedSampleCount;
    psoDesc.RasterizerState.ConservativeRaster = ConvertCoservativeRasterizationFrom(m_conservativeRasterEnable);

    // デプスステンシルステート
    psoDesc.DepthStencilState.DepthEnable = m_depthEnable;
    psoDesc.DepthStencilState.DepthWriteMask = ConvertDepthWriteMastFrom(m_depthWriteEnable);
    psoDesc.DepthStencilState.DepthFunc = ConvertFrom(m_depthFunc);
    psoDesc.DepthStencilState.StencilEnable = m_stencilEnable;
    psoDesc.DepthStencilState.StencilReadMask = m_stencilReadMask;
    psoDesc.DepthStencilState.StencilWriteMask = m_stencilWriteMask;
    psoDesc.DepthStencilState.FrontFace.StencilFailOp = ConvertFrom(m_frontFaceStencilFailOp);
    psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = ConvertFrom(m_frontFaceStencilDepthFailOp);
    psoDesc.DepthStencilState.FrontFace.StencilPassOp = ConvertFrom(m_frontFaceStencilPassOp);
    psoDesc.DepthStencilState.FrontFace.StencilFunc = ConvertFrom(m_frontFaceStencilFunc);
    psoDesc.DepthStencilState.BackFace.StencilFailOp = ConvertFrom(m_backFaceStencilFailOp);
    psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = ConvertFrom(m_backFaceStencilDepthFailOp);
    psoDesc.DepthStencilState.BackFace.StencilPassOp = ConvertFrom(m_backFaceStencilPassOp);
    psoDesc.DepthStencilState.BackFace.StencilFunc = ConvertFrom(m_backFaceStencilFunc);

    // 出力マージャー (OM)
    psoDesc.NumRenderTargets = m_numRenderTargets;
    memset(psoDesc.RTVFormats, 0, sizeof(psoDesc.RTVFormats));
    for (UINT i = 0; i < psoDesc.NumRenderTargets; i++)
    {
        psoDesc.RTVFormats[i] = ConvertFrom(m_renderTargetFormats[i]);
    }
    psoDesc.DSVFormat = ConvertFrom(m_depthStencilFormat);

    // その他
    psoDesc.SampleDesc.Count = m_multiSampleCount;
    psoDesc.SampleDesc.Quality = m_multiSampleQuality;
    psoDesc.SampleMask = m_blendSampleMask;
    psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;


    //---------------------------------------------------------------------------------------------------------------------------------------------
    // パイプラインステートの作成
    //      第1引数 : [in] 作成に関する詳細情報
    //      第2引数 : [in] インターフェースID
    //      第3引数 : [out] パイプラインステートを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //---------------------------------------------------------------------------------------------------------------------------------------------
    if (FAILED(GraphicsEngine::Instance().GetD3D12Device()->CreateGraphicsPipelineState(&psoDesc, IID_ID3D12PipelineState, (void**)ppD3D12PipelineState)))
    {
        printf("[失敗] パイプラインステートの作成\n");
        assert(0);
    }
    printf("[成功] パイプラインステートの作成 (アドレス: 0x%p)\n", *ppD3D12PipelineState);

    m_isBegun = false;
}


#if 0
//---------------------------------------------------------------------------------------------------------------------------------------------
// パイプラインステート (英: Pipeline State)
//---------------------------------------------------------------------------------------------------------------------------------------------

// パイプラインステートを作成する為の詳細情報
D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
memset(&psoDesc, 0, sizeof(psoDesc));                                                   // 全てを0で埋める
psoDesc.pRootSignature = d3d12RootSignature;                                            // ルートシグネチャ
psoDesc.InputLayout.NumElements = _countof(InputElementDescs);                          // 入力要素詳細配列の要素数
psoDesc.InputLayout.pInputElementDescs = InputElementDescs;                             // 入力要素詳細配列のアドレス
psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;                  // インデックスバッファ内でのストリップカット値
psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;                 // プリミティブトポロジータイプ
psoDesc.VS.pShaderBytecode = vertexShader->GetBufferPointer();                          // 頂点シェーダーバイトコード
psoDesc.VS.BytecodeLength = vertexShader->GetBufferSize();                              // 頂点シェーダーバイトコードのバイト数
psoDesc.HS.pShaderBytecode = nullptr;                                                   // ハルシェーダーバイトコード
psoDesc.HS.BytecodeLength = 0;                                                          // ハルシェーダーバイトコードのバイト数
psoDesc.DS.pShaderBytecode = nullptr;                                                   // ドメインシェーダーバイトコード
psoDesc.DS.BytecodeLength = 0;                                                          // ドメインシェーダーバイトコードのバイト数
psoDesc.GS.pShaderBytecode = nullptr;                                                   // ジオメトリシェーダーバイトコード
psoDesc.GS.BytecodeLength = 0;                                                          // ジオメトリシェーダーバイトコードのバイト数
psoDesc.PS.pShaderBytecode = pixelShader->GetBufferPointer();                           // ピクセルシェーダーバイトコード
psoDesc.PS.BytecodeLength = pixelShader->GetBufferSize();                               // ピクセルシェーダーバイトコードのバイト数
psoDesc.SampleDesc.Count = 1;                                                           // マルチサンプリング数
psoDesc.SampleDesc.Quality = 0;                                                         // マルチサンプリング品質
psoDesc.SampleMask = UINT_MAX;                                                          // サンプリングマスク
psoDesc.RasterizerState.AntialiasedLineEnable = FALSE;                                  // 線プリミティブ描画時のアンチエイリアス処理の有効/無効
psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;                                // 陰面処理に関する設定
psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;                               // ポリゴン内部の塗りつぶしに関する設定
psoDesc.RasterizerState.DepthClipEnable = TRUE;                                         // 深度クリップの有効/無効
psoDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;                           // 深度バイアス値
psoDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;                // 深度バイアス値のクランプ処理時に使用される値
psoDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;   // 深度バイアス値の乗算係数
psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // 保守的ラスターの有効/無効
psoDesc.RasterizerState.MultisampleEnable = FALSE;                                      // マルチサンプリング処理の有効/無効
psoDesc.RasterizerState.FrontCounterClockwise = FALSE;                                  // 反時計回りに頂点を配置した側をポリゴンの表面とするか？
psoDesc.RasterizerState.ForcedSampleCount = 0;                                          // (0,1,2,4,8,16のいずれか)
psoDesc.BlendState.AlphaToCoverageEnable = TRUE;                                        // アルファトゥカバレッジの有効/無効
psoDesc.BlendState.IndependentBlendEnable = FALSE;                                      // レンダーターゲットごとのブレンド処理の有効/無効
psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;                                  // レンダーターゲット[0]のブレンド処理の有効/無効
psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;                    // レンダーターゲット[0]のソースカラー(Rs, Gs, Bs)に対する乗算設定
psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;               // レンダーターゲット[0]のデストカラー(Rd, Gd, Bd)に対する乗算設定
psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;                        // レンダーターゲット[0]のソースカラーとデストカラーの演算設定
psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;                     // レンダーターゲット[0]のソースアルファ(As)に対する乗算設定
psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;                   // レンダーターゲット[0]のデストアルファ(Ad)に対する乗算設定
psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;                   // レンダーターゲット[0]のソースアルフとデストアルフの演算設定
psoDesc.BlendState.RenderTarget[0].LogicOpEnable = FALSE;                               // レンダーターゲット[0]の論理演算処理の有効/無効
psoDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;                       // レンダーターゲット[0]の論理演算設定
psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// カラーチャンネルごとの書き込みフラグ
psoDesc.DepthStencilState.DepthEnable = TRUE;                                           // 深度値比較の有効/無効
psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;                       // 深度値比較関数
psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;                  // 深度値書き込み時のビットマスク
psoDesc.DepthStencilState.StencilEnable = FALSE;                                        // ステンシル値の比較の有効/無効
psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;            // ステンシル値の読み取り時ビットマスク
psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;          // ステンシル値の書き込み時ビットマスク
psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;         // 
psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;         // 
psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;              // 
psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;              // 
psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;          // 
psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;          // 
psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;               // 
psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;               // 
psoDesc.NumRenderTargets = 1;                                                           // レンダーターゲット数
psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;                                     // レンダーターゲット[0]のピクセルフォーマット
psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;                                              // 深度ステンシルフォーマット
psoDesc.CachedPSO.pCachedBlob = nullptr;                                                // キャッシュされたパイプラインステートオブジェクト
psoDesc.CachedPSO.CachedBlobSizeInBytes = 0;                                            // キャッシュされたパイプラインステートオブジェクトのバイト数
psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;                                         // その他のフラグ
psoDesc.NodeMask = 0;                                                                   // 単一アダプターの場合は 0 または 1

//---------------------------------------------------------------------------------------------------------------------------------------------
// パイプラインステートの作成
//      第1引数 : [in] 作成に関する詳細情報
//      第2引数 : [in] インターフェースID
//      第3引数 : [out] パイプラインステートを受け取る変数のアドレス
//       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
//---------------------------------------------------------------------------------------------------------------------------------------------
ID3D12PipelineState* d3d12PipelineState;
if (FAILED(GraphicsEngine::Instance().GetD3D12Device()->CreateGraphicsPipelineState(&psoDesc, IID_ID3D12PipelineState, (void**)&d3d12PipelineState)))
{
    printf("[失敗] パイプラインステートの作成\n");
    assert(0);
}
printf("[成功] パイプラインステートの作成 (アドレス: 0x%p)\n", d3d12PipelineState);
#endif