#pragma once
#include <d3d12.h>
#include <vector>

// 前方宣言
class RootParameter;
class StaticSampler;

class RootSignatureBuilder
{
private:
    std::vector<D3D12_ROOT_PARAMETER1> m_rootParameters;
    std::vector<D3D12_STATIC_SAMPLER_DESC> m_staticSamplers;
    bool m_isBegun;

public:
    // コンストラクタ
    RootSignatureBuilder();

    // ルートパラメーターとスタティックサンプラーの記録を開始します。
    void Begin();

    // ルートパラメーター配列の要素数を返します。
    uint32_t GetNumRootParameters() const;

    // ルートパラメーターを追加します。
    uint32_t AddRootParameter(const RootParameter& rootParameter);

    // スタティックサンプラー数を返します。
    uint32_t GetNumStaticSampler() const;

    // スタティックサンプラーを追加します。
    void AddStaticSampler(const StaticSampler& staticSampler);

    // ルートパラメーターとスタティックサンプラーの記録を終了してビルドします。
    void End(ID3D12RootSignature** ppD3D12RootSignature);
};

