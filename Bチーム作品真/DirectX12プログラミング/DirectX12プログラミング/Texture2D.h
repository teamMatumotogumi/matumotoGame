#pragma once
#include "Texture.h"
#include <d3d12.h>

enum class TextureFormat
{
    RGBA32,
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 2Dテクスチャクラス
// 
//      ・2D/3Dゲームにおいて最もよく利用されるタイプのテクスチャ。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class Texture2D : public Texture
{
private:
    TextureFormat           m_format;
    ID3D12Resource*         m_nativeTexture;
    ID3D12DescriptorHeap*   m_descriptorHeap;

protected:
    // コンストラクタ
    Texture2D();

    // 仮想デストラクタ
    virtual ~Texture2D() override;

    // 中間バッファを作成します。
    static ID3D12Resource* CreateIntermediateBuffer(ID3D12Device* d3d12Device, uint64_t byteWidth);

public:
    // 画像ファイルをロードしてテクスチャを作成します。
    static Texture2D* FromFile(const wchar_t* textureFilePath, ID3D12GraphicsCommandList* commandList = nullptr);

    // ピクセルフォーマットを取得します。
    TextureFormat GetFormat() const { return m_format; }

    // テクスチャリソースへのネイティブポインタを取得します。
    void* GetNativeTexturePtr() const override { return m_nativeTexture; }

    // ディスクリプタヒープを取得します。
    ID3D12DescriptorHeap* GetDescriptorHeap() const { return m_descriptorHeap; }
};
