#pragma once
#include <cstdint>
#include <vector>
#include "RenderTargetBlend.h"

enum class PrimitiveTopology
{
    Undefined,
    Point,
    Line,
    Triangle,
    Patch,
};


enum class IndexBufferStripCutValue : unsigned int
{
    Disable,                    // 無効
    UInt16Max = 0xFFFF,         // 符号なし16ビット整数型の最大値 (0xFFFF)
    UInt32Max = 0xFFFFFFFF,     // 符号なし32ビット整数型の最大値 (0xFFFFFFFF)
};


enum class RenderTargetFormat
{
    R32G32B32A32_Float,
    R32G32B32A32_UInt,
    R32G32B32A32_SInt,
    R16G16B16A16_Float,
    R16G16B16A16_UNorm,
    R16G16B16A16_UInt,
    R16G16B16A16_SNorm,
    R16G16B16A16_SInt,
    R8G8B8A8_UNorm,
    R8G8B8A8_UInt,
    R8G8B8A8_SNorm,
    R8G8B8A8_SInt,
    Default = R8G8B8A8_UNorm,
};


enum class FillMode
{
    Wireframe,
    Solid,
};


enum class CullMode
{
    Off,
    Front,
    Back,
};


enum class ComparisonFunc
{
    Never,
    Less,
    Equal,
    LessEqual,
    Greater,
    NotEqual,
    GreaterEqual,
    Always
};


enum class StencilOperation
{
    Keep,                   // 現在のステンシル値を維持します。
    Zero,                   // ステンシルバッファの値を0にセット
    Replace,                // ステンシルバッファの値を参照値と入れ替えます。 (シェーダー内で指定).
    IncrementSaturate,      // ステンシルバッファの値をインクリメントします。(255でクランプされます)
    DecrementSaturate,      // ステンシルバッファの値をデクリメントします。 (0でクランプされます)
    Invert,                 // ステンシルバッファの値をビット反転します。.
    Increment,              // ステンシルバッファの値をインクリメントします。 (255をインクリメントした場合は0にラップされます)
    Decrement,              // ステンシルバッファの値をデクリメントします。 (0をデクリメントした場合は255にラップされます)
};


class RootSignature;
class InputLayout;
class ShaderBytecode;
class GraphicsPipeline;
enum class DepthStencilFormat;

//---------------------------------------------------------------------------------------------------------------------------------------------
// [PSOビルド時に検証される項目]
//
//      ・ハルシェーダーとドメインシェーダーに有効な値が設定されている時、プリミティブトポロジーが「パッチ」に設定されているかどうか。
//      ・ブレンドステートがレンダーターゲットフォーマットと互換性があるかどうか。
//      ・ピクセルシェーダーからの出力データがレンダーターゲットフォーマットと互換性があるかどうか。
//      ・マルチサンプリングの設定がレンダーターゲットフォーマット/デプスステンシルフォーマットと互換性があるかどうか。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class PipelineStateBuilder
{
protected:

    bool m_isBegun;

    // 入力アセンブラ (IA)
    ID3D12RootSignature*        m_rootSignature;
    PrimitiveTopology           m_primitiveTopology;
    IndexBufferStripCutValue    m_indexBufferStripCutValue;
    uint32_t                    m_numInputElementDescs;
    D3D12_INPUT_ELEMENT_DESC    m_inputElementDescs[16];

    // プログラマブルシェーダー
    ShaderBytecode*             m_VS;   // 頂点シェーダー
    ShaderBytecode*             m_DS;   // ドメインシェーダー
    ShaderBytecode*             m_HS;   // ハルシェーダー
    ShaderBytecode*             m_GS;   // ジオメトリシェーダー
    ShaderBytecode*             m_PS;   // ピクセルシェーダー

    // マルチサンプリング
    uint32_t                    m_multiSampleCount;
    uint32_t                    m_multiSampleQuality;

    // ブレンドステート (BS)
    bool                        m_alphaToCoverageEnable;
    bool                        m_independentBlendEnable;
    uint32_t                    m_blendSampleMask;
    RenderTargetBlend           m_renderTargetBlend[8];

    // ラスタライザーステート (RS)
    FillMode                    m_fillMode;
    CullMode                    m_cullMode;
    bool                        m_frontCounterClockwise;
    int32_t                     m_depthBias;
    float                       m_depthBiasClamp;
    float                       m_slopeScaledDepthBias;
    bool                        m_depthClipEnable;
    bool                        m_multisampleEnable;
    bool                        m_antialiasedLineEnable;
    uint32_t                    m_forcedSampleCount;
    bool                        m_conservativeRasterEnable;

    // 深度ステンシルステート
    bool                        m_depthEnable;
    bool                        m_depthWriteEnable;
    ComparisonFunc              m_depthFunc;
    bool                        m_stencilEnable;
    uint8_t                     m_stencilReadMask;
    uint8_t                     m_stencilWriteMask;
    StencilOperation            m_frontFaceStencilFailOp;
    StencilOperation            m_frontFaceStencilDepthFailOp;
    StencilOperation            m_frontFaceStencilPassOp;
    ComparisonFunc              m_frontFaceStencilFunc;
    StencilOperation            m_backFaceStencilFailOp;
    StencilOperation            m_backFaceStencilDepthFailOp;
    StencilOperation            m_backFaceStencilPassOp;
    ComparisonFunc              m_backFaceStencilFunc;

    // 出力マージャー (OM)
    uint32_t                    m_numRenderTargets;
    RenderTargetFormat          m_renderTargetFormats[8];
    DepthStencilFormat          m_depthStencilFormat;

private:
    // デフォルト値にリセットします。
    void ResetToDefault();

public:
    // コンストラクタ
    PipelineStateBuilder();

    // 仮想デストラクタ
    virtual ~PipelineStateBuilder() = default;

    // 
    virtual void Begin(bool resetToDefault);

    // ルートシグネチャを設定します。
    void SetRootSignature(ID3D12RootSignature* rootSignature) { m_rootSignature = rootSignature; }

    // ルートシグネチャを設定します。
    ID3D12RootSignature* GetRootSignature() const { return m_rootSignature; }

    //--------------------------------------------------------------------------------------------------------------------
    // 入力アセンブラステージ (IA)
    //--------------------------------------------------------------------------------------------------------------------

    // 入力要素詳細情報を設定します。
    void IASetInputElementDescs(uint32_t numInputElementDescs, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs);

    // プリミティブトポロジーを設定します。
    void IASetPrimitiveTopology(PrimitiveTopology primitiveTopology) { m_primitiveTopology = primitiveTopology; }

    // プリミティブトポロジーを取得します。
    PrimitiveTopology IAGetPrimitiveTopology() const { return m_primitiveTopology; }

    // インデックスバッファ内の三角形ストリップのリスタートを意味する値を設定します。
    void IASetIBStripCutValueType(IndexBufferStripCutValue value) { m_indexBufferStripCutValue = value; }

    // インデックスバッファ内の三角形ストリップのリスタートを意味する値を取得します。
    IndexBufferStripCutValue IAGetIBStripCutValueType() const { return m_indexBufferStripCutValue; }

    //--------------------------------------------------------------------------------------------------------------------
    // 頂点シェーダーステージ (VS)
    //--------------------------------------------------------------------------------------------------------------------

    // 頂点シェーダーを設定します。
    void VSSetShader(ShaderBytecode* vertexShader) { m_VS = vertexShader; }

    // 頂点シェーダーを取得します。
    ShaderBytecode* VSGetShader() const { return m_VS; }

    //--------------------------------------------------------------------------------------------------------------------
    // ドメインシェーダーステージ (DS)
    //--------------------------------------------------------------------------------------------------------------------

    // ドメインシェーダーを設定します。
    void DSSetShader(ShaderBytecode* domainShader) { m_DS = domainShader; }

    // ドメインシェーダーを取得します。
    ShaderBytecode* DSGetShader() const { return m_DS; }

    //--------------------------------------------------------------------------------------------------------------------
    // ハルシェーダーステージ (HS)
    //--------------------------------------------------------------------------------------------------------------------

    // ハルシェーダーを設定します。
    void HSSetShader(ShaderBytecode* hullShader) { m_HS = hullShader; }

    // ハルシェーダーを取得します。
    ShaderBytecode* HSGetShader() const { return m_HS; }

    //--------------------------------------------------------------------------------------------------------------------
    // ジオメトリシェーダーステージ (GS)
    //--------------------------------------------------------------------------------------------------------------------

    // ジオメトリシェーダーを設定します。
    void GSSetShader(ShaderBytecode* geometryShader) { m_GS = geometryShader; }

    // ジオメトリシェーダーを取得します。
    ShaderBytecode* GSGetShader() const { return m_GS; }

    //--------------------------------------------------------------------------------------------------------------------
    // ピクセルシェーダーステージ (PS)
    //--------------------------------------------------------------------------------------------------------------------

    // ピクセルシェーダーを設定します。
    void PSSetShader(ShaderBytecode* pixelShader) { m_PS = pixelShader; }

    // ピクセルシェーダーを取得します。
    ShaderBytecode* PSGetShader() const { return m_PS; }


    //--------------------------------------------------------------------------------------------------------------------
    // マルチサンプリング
    //--------------------------------------------------------------------------------------------------------------------

    // マルチサンプル数を設定します。(count = 1)の場合は無効。
    void SetMultiSampleCount(uint32_t count = 1) { m_multiSampleCount = count; }

    // マルチサンプル数を取得します。(count = 1)の場合は無効。
    uint32_t GetMultiSampleCount() const { return m_multiSampleCount; }

    // マルチサンプル品質を表す値を設定します。(quality = 0)の場合は無効。
    void SetMultiSampleQuality(uint32_t quality = 0) { m_multiSampleQuality = quality; }

    // マルチサンプル品質を表す値を取得します。(quality = 0)の場合は無効。
    uint32_t GetMultiSampleQuality() const { return m_multiSampleQuality; }

    // サンプルマスクを設定します。
    void SetBlendSampleMask(uint32_t mask) { m_blendSampleMask = mask; }

    // サンプルマスクを取得します。
    uint32_t GetBlendSampleMask() const { return m_blendSampleMask; }

    //--------------------------------------------------------------------------------------------------------------------
    // ブレンドステート (BS)
    //--------------------------------------------------------------------------------------------------------------------

    // アルファトゥカバレッジを有効にする場合は true を渡します。
    void BSSetAlphaToCoverageEnable(bool enable) { m_alphaToCoverageEnable = enable; }

    // アルファトゥカバレッジが有効な場合は true を返します。
    bool BSGetAlphaToCoverageEnable() const { return m_alphaToCoverageEnable; }

    // レンダーターゲットごとに異なるブレンディングを行いたい場合は true を渡します。
    void BSSetIndependentEnable(bool enable) { m_independentBlendEnable = enable; }

    // レンダーターゲットごとに異なるブレンディングを行う場合は true を返します。
    bool BSGetIndependentEnable() const { return m_independentBlendEnable; }

    // index番目のレンダーターゲットに対するブレンドを設定します。
    void BSSetRenderTargetBlend(uint32_t index, const RenderTargetBlend& blendDesc) { m_renderTargetBlend[index] = blendDesc; }

    // index番目のレンダーターゲットに対するブレンドを設定します。
    const RenderTargetBlend& BSGetRenderTargetBlend(uint32_t index) const { return m_renderTargetBlend[index]; }

    // index番目のレンダーターゲットに対するブレンド設定を有効にしたい場合は true を渡します。
    void BSSetBlendEnable(uint32_t index, bool enable) { m_renderTargetBlend[index].BlendEnable = enable; }

    // index番目のレンダーターゲットに対するブレンド設定が有効な場合は true を返します。
    bool BSGetBlendEnable(uint32_t index) const { return m_renderTargetBlend[index].BlendEnable; }

    // index番目のレンダーターゲットに対するロジック設定を有効にしたい場合は true を渡します。
    void BSSetLogicOperationEnable(uint32_t index, bool enable) { m_renderTargetBlend[index].LogicOpEnable = enable; }

    // index番目のレンダーターゲットに対するロジック設定が有効な場合は true を返します。
    bool BSGetLogicOperationEnable(uint32_t index) const { return m_renderTargetBlend[index].LogicOpEnable; }

    // index番目のレンダーターゲットに対するブレンドファクター(Source)を設定します。
    void BSSetSource(uint32_t index, BlendFactor factor) { m_renderTargetBlend[index].SrcBlend = factor; }

    // index番目のレンダーターゲットに対するブレンドファクター(Source)を取得します。
    BlendFactor BSGetSource(uint32_t index) const { return m_renderTargetBlend[index].SrcBlend; }

    // index番目のレンダーターゲットに対するブレンドファクター(SourceAlpha)を設定します。
    void BSSetSourceAlpha(uint32_t index, BlendFactor factor) { m_renderTargetBlend[index].SrcBlendAlpha = factor; }

    // index番目のレンダーターゲットに対するブレンドファクター(SourceAlpha)を取得します。
    BlendFactor BSGetSourceAlpha(uint32_t index) const { return m_renderTargetBlend[index].SrcBlendAlpha; }

    // index番目のレンダーターゲットに対するブレンドファクター(Destination)を設定します。
    void BSSetDestination(uint32_t index, BlendFactor factor) { m_renderTargetBlend[index].DestBlend = factor; }

    // index番目のレンダーターゲットに対するブレンドファクター(Destination)を取得します。
    BlendFactor BSGetDestination(uint32_t index) const { return m_renderTargetBlend[index].DestBlend; }

    // index番目のレンダーターゲットに対するブレンドファクター(DestinationAlpha)を設定します。
    void BSSetDestinationAlpha(uint32_t index, BlendFactor factor) { m_renderTargetBlend[index].DestBlendAlpha = factor; }

    // index番目のレンダーターゲットに対するブレンドファクター(DestinationAlpha)を取得します。
    BlendFactor BSGetDestinationAlpha(uint32_t index) const { return m_renderTargetBlend[index].DestBlendAlpha; }

    // index番目のレンダーターゲットに対するブレンドオペレーションを設定します。
    void BSSetBlendOperation(uint32_t index, BlendOperation operation) { m_renderTargetBlend[index].BlendOp = operation; }

    // index番目のレンダーターゲットに対するブレンドオペレーションを取得します。
    BlendOperation BSGetBlendOperation(uint32_t index) const { return m_renderTargetBlend[index].BlendOp; }

    // index番目のレンダーターゲットに対するブレンドオペレーション(Alpha)を設定します。
    void BSSetBlendOperationAlpha(uint32_t index, BlendOperation operation) { m_renderTargetBlend[index].BlendOpAlpha = operation; }

    // index番目のレンダーターゲットに対するブレンドオペレーション(Alpha)を取得します。
    BlendOperation BSGetBlendOperationAlpha(uint32_t index) const { return m_renderTargetBlend[index].BlendOpAlpha; }

    // index番目のレンダーターゲットに対するロジックオペレーションを設定します。
    void BSSetLogicOperation(uint32_t index, LogicOperation operation) { m_renderTargetBlend[index].LogicOp = operation; }

    // index番目のレンダーターゲットに対するロジックオペレーションを取得します。
    LogicOperation BSGetLogicOperation(uint32_t index) const { return m_renderTargetBlend[index].LogicOp; }

    // index番目のレンダーターゲットに対するカラー書き込みマスクを設定します。
    void BSSetRenderTargetWriteMask(uint32_t index, ColorWriteEnable mask) { m_renderTargetBlend[index].RenderTargetWriteMask = mask; }

    // index番目のレンダーターゲットに対するカラー書き込みマスクを取得します。
    ColorWriteEnable BSGetRenderTargetWriteMask(uint32_t index) const { return m_renderTargetBlend[index].RenderTargetWriteMask; }

    //--------------------------------------------------------------------------------------------------------------------
    // 出力マージャーステージ (OM)
    //--------------------------------------------------------------------------------------------------------------------

    // 深度比較を有効にする場合は true を渡します。
    void OMSetDepthEnable(bool enable) { m_depthEnable = enable; }

    // 深度比較が有効な場合は true を返します。
    bool OMGetDepthEnable() const { return m_depthEnable; }

    // 深度書き込みを有効にする場合は true を渡します。
    void OMSetDepthWriteEnable(bool enable) { m_depthWriteEnable = enable; }

    // 深度書き込みが有効な場合は true を返します。
    bool OMGetDepthWriteEnable() const { return m_depthWriteEnable; }

    // 深度比較関数を設定します。
    void OMSetDepthFunc(ComparisonFunc func) { m_depthFunc = func; }

    // 深度比較関数を取得します。
    ComparisonFunc OMGetDepthFunc() const { return m_depthFunc; }

    // ステンシルを有効にする場合は true を渡します。
    void OMSetStencilEnable(bool enable) { m_stencilEnable = enable; }

    // ステンシルが有効な場合は true を返します。
    bool OMGetStencilEnable() const { return m_stencilEnable; }

    // ステンシル値読み込みマスク設定します。
    void OMSetStencilReadMask(uint8_t mask) { m_stencilReadMask = mask; }

    // ステンシル値読み込みマスク取得します。
    uint8_t OMGetStencilReadMask() const { return m_stencilReadMask; }

    // ステンシル値書き込みマスク設定します。
    void OMSetStencilWriteMask(uint8_t mask) { m_stencilWriteMask = mask; }

    // ステンシル値書き込みマスク取得します。
    uint8_t OMGetStencilWriteMask() const { return m_stencilWriteMask; }

    // レンダーターゲット数を設定します。
    void OMSetNumRenderTargets(uint32_t numRenderTargets) { m_numRenderTargets = numRenderTargets; }

    // レンダーターゲット数を取得します。
    uint32_t OMGetNumRenderTargets() const { return m_numRenderTargets; }

    // index番目のレンダーターゲットのフォーマットを設定します。
    void OMSetRenderTargetFormat(uint32_t index, RenderTargetFormat format) { m_renderTargetFormats[index] = format; }

    // index番目のレンダーターゲットのフォーマットを取得します。
    RenderTargetFormat OMGetRenderTargetFormat(uint32_t index) const { return m_renderTargetFormats[index]; }

    // デプスステンシルフォーマットを設定します。
    void OMSetDepthStencilFormat(DepthStencilFormat format) { m_depthStencilFormat = format; }

    // デプスステンシルフォーマットを取得します。
    DepthStencilFormat OMGetDepthStencilFormat() const { return m_depthStencilFormat; }

    //--------------------------------------------------------------------------------------------------------------------
    // ラスタライザーステージ (RS)
    //--------------------------------------------------------------------------------------------------------------------

    // フィルモードを設定します。
    void RSSetFillMode(FillMode mode) { m_fillMode = mode; }

    // フィルモードを設定します。
    FillMode RSGetFillMode() const { return m_fillMode; }

    // フェースカリングモードを設定します。
    void RSSetCullMode(CullMode mode) { m_cullMode = mode; }

    // フェースカリングモードを設定します。
    CullMode RSGetCullMode() const { return m_cullMode; }

    // 頂点が反時計回りに配置される側が「前面」の場合は true を設定します。
    void RSSetFrontCounterClockwise(bool enable) { m_frontCounterClockwise = enable; }

    // 頂点が反時計回りに配置される側が「前面」の場合は true を返します。
    bool RSGetFrontCounterClockwise() const { return m_frontCounterClockwise; }

    // 深度バイアス値を設定します。
    void RSSetDepthBias(int32_t value) { m_depthBias = value; }

    // 深度バイアス値を取得します。
    int32_t RSGetDepthBias() const { return m_depthBias; }

    // 深度バイアスをクランプする値を設定します。
    void RSSetDepthBiasClamp(float value) { m_depthBiasClamp = value; }

    // 深度バイアスをクランプする値を取得します。
    float RSGetDepthBiasClamp() const { return m_depthBiasClamp; }

    // 深度傾斜値を設定します。
    void RSSetSlopeScaledDepthBias(float value) { m_slopeScaledDepthBias = value; }

    // 深度傾斜値を取得します。
    float RSGetSlopeScaledDepthBias() const { return m_slopeScaledDepthBias; }

    // 深度クリップを有効にする場合は true を渡します。
    void RSSetDepthClipEnable(bool enable) { m_depthClipEnable = enable; }

    // 深度クリップを有効が有効な場合は true を返します。
    bool RSGetDepthClipEnable() const { return m_depthClipEnable; }

    // マルチサンプリング(MSAA)を有効にする場合は true を渡します。
    void RSSetMultisampleEnable(bool enable) { m_multisampleEnable = enable; }

    // マルチサンプリング(MSAA)が有効な場合は true を返します。
    bool RSGetMultisampleEnable() const { return m_multisampleEnable; }

    // アンチエイリアシングされたラインを有効にする場合は true を渡します。
    void RSSetAntialiasedLineEnable(bool enable) { m_antialiasedLineEnable = enable; }

    // アンチエイリアシングされたラインが有効な場合は true を返します。
    bool RSGetAntialiasedLineEnable() const { return m_antialiasedLineEnable; }

    // 保守的なラスター化モードを有効にする場合は true を渡します。
    void RSSetConservativeRasterEnable(bool enable) { m_conservativeRasterEnable = enable; }

    // 保守的なラスター化モードが有効な場合は true を返します。
    bool RSGetConservativeRasterEnable() const { return m_conservativeRasterEnable; }

    // サンプルカウントの強制値を設定します。
    // 有効な値は(0,1,2,4,8,16)のいずれかで、0を指定した場合は強制しません。
    void RSSetForcedSampleCount(uint32_t count) { m_forcedSampleCount = count; }

    // サンプルカウントの強制値を取得します。
    uint32_t RSGetForcedSampleCount() const { return m_forcedSampleCount; }

    // 内部ステートに従ってビルドし、新規パイプラインステートを作成します。
    void End(ID3D12PipelineState** ppD3D12PipelineState);
};

