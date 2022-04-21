#include "Precompiled.h"
#include "RootSignatureBuilder.h"
#include "RootParameter.h"
#include "StaticSampler.h"
#include "GraphicsEngine.h"


RootSignatureBuilder::RootSignatureBuilder()
    : m_isBegun(false)
{
}

void RootSignatureBuilder::Begin()
{
    assert(!m_isBegun);
    m_rootParameters.clear();
    m_staticSamplers.clear();
    m_isBegun = true;
}

uint32_t RootSignatureBuilder::GetNumRootParameters() const
{
    return (uint32_t)m_rootParameters.size();
}

uint32_t RootSignatureBuilder::AddRootParameter(const RootParameter& rootParameter)
{
    m_rootParameters.push_back(rootParameter);
    return (uint32_t)m_rootParameters.size() - 1;
}

uint32_t RootSignatureBuilder::GetNumStaticSampler() const
{
    return (uint32_t)m_staticSamplers.size();
}

void RootSignatureBuilder::AddStaticSampler(const StaticSampler& staticSampler)
{
    m_staticSamplers.push_back(staticSampler);
}

void RootSignatureBuilder::End(ID3D12RootSignature** ppD3D12RootSignature)
{
    assert(m_isBegun);

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ルートシグネチャを作成する為の詳細情報
    //---------------------------------------------------------------------------------------------------------------------------------------------
    D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc;
    memset(&desc, 0, sizeof(desc));                                         // 全て0で埋めておく
    desc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;                          // ルートシグネチャ バージョン 1.1
    desc.Desc_1_1.NumParameters = GetNumRootParameters();                   // ルートパラメーター配列の要素数
    desc.Desc_1_1.pParameters = m_rootParameters.data();                    // ルートパラメーター配列
    desc.Desc_1_1.NumStaticSamplers = GetNumStaticSampler();                // スタティックサンプラー配列の要素数
    desc.Desc_1_1.pStaticSamplers = m_staticSamplers.data();                // スタティックサンプラー配列
    desc.Desc_1_1.Flags                                                     // 各種オプションフラグ
        = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT      // 入力アセンブラ(IA)の入力レイアウトを許可する
        | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS            // ハルシェーダーによるルートアクセスを拒否する
        | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS          // ドメインシェーダーによるルートアクセスを拒否する
        | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;       // ジオメトリシェーダーによるルートアクセスを拒否する

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ルートシグネチャのシリアライズ(直列化)
    // 
    //      データをシリアライズ(直列化)することによってファイル等に書き出したり、ファイル等から読み込んだりすることが可能になる。
    // 
    //      第1引数 : [in] 作成に関する詳細情報
    //      第2引数 : [in] 作成に関する詳細情報のバージョン (2021年現在では 1.0 または 1.1 のどちらか)
    //      第3引数 : [out] シリアライズされたルートシグネチャバイトコードを受け取るID3DBlob*型変数のアドレス
    //      第4引数 : [out] シリアライズに失敗した際のエラー文字列を受け取るID3DBlob*型変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //---------------------------------------------------------------------------------------------------------------------------------------------
    ID3DBlob* blobWithRootSignature;
    ID3DBlob* serializeError;
    if (FAILED(D3D12SerializeVersionedRootSignature(&desc, &blobWithRootSignature, &serializeError)))
    {
        printf("[失敗] ルートシグネチャのシリアライズ\n");
        if (serializeError)
        {
            serializeError->Release();
        }
        assert(0);
    }
    printf("[成功] ルートシグネチャのシリアライズ\n");

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ルートシグネチャオブジェクトの作成
    // 
    //      第1引数 : [in] ノードマスク (単一アダプターの場合は 0 または 1)
    //      第2引数 : [in] シリアライズ(直列化)されたルートシグネチャバイトコード
    //      第3引数 : [in] シリアライズ(直列化)されたルートシグネチャバイトコードのサイズ(単位はバイト)
    //      第4引数 : [in] インターフェースID
    //      第5引数 : [out] ルートシグネチャを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //---------------------------------------------------------------------------------------------------------------------------------------------
    if (FAILED(GraphicsEngine::Instance().GetD3D12Device()->CreateRootSignature(0, blobWithRootSignature->GetBufferPointer(), blobWithRootSignature->GetBufferSize(), IID_ID3D12RootSignature, (void**)ppD3D12RootSignature)))
    {
        printf("[失敗] ルートシグネチャの作成\n");
        assert(0);
    }
    printf("[成功] ルートシグネチャの作成 (アドレス: 0x%p)\n", *ppD3D12RootSignature);
    blobWithRootSignature->Release();

    m_isBegun = false;
}
