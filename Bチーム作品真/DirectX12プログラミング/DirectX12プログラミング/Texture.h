#pragma once
#include "Object.h"
#include "FilterMode.h"
#include "TextureWrapMode.h"


enum class TextureDimension
{
    Unknown,    // テクスチャタイプが初期化されていないか、または不明です。
    None,       // テクスチャが指定されていません。
    Any,        // 全てのテクスチャタイプ。
    Tex2D,      // 2Dテクスチャ(Texture2D)。
    Tex3D,      // 3Dボリュームテクスチャ (Texture3D)。
    Cube,       // キューブマップテクスチャ。
    Tex2DArray, // 2D配列テクスチャ (Texture2DArray)。
    CubeArray,  // キューブマップ配列テクスチャ (CubemapArray)。
};


//---------------------------------------------------------------------------------------------------------------------------------------------
// テクスチャクラス
// 
//      ・全てのテクスチャの基底クラス
//      ・次元、サイズ、ラップモード等の情報を保持する。
//      ・派生クラスは「Texture2D」「RenderTexture」等。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class Texture : public Object
{
protected:
    std::string m_name;
    TextureDimension m_dimension;
    int m_width;
    int m_height;
    int m_anisoLevel;
    TextureWrapMode m_wrapModeU;
    TextureWrapMode m_wrapModeV;
    TextureWrapMode m_wrapModeW;
    FilterMode m_filterMode;
    float m_mipMapBias;
    bool m_isReadable;

protected:
    // コンストラクタ
    Texture();

    // 仮想デストラクタ
    virtual ~Texture() override;

public:
    // このオブジェクトの識別名を設定します。
    void SetName(const std::string& name) override { m_name = name; }

    // このオブジェクトの識別名を取得します。
    const std::string& GetName() const override { return m_name; }

    // ピクセル単位のテクスチャの幅を取得します。
    int GetWidth() const { return m_width; }

    // ピクセル単位のテクスチャの高さを取得します。
    int GetHeight() const { return m_height; }

    //     テクスチャの異方性フィルタリングのレベルを取得します。
    int GetAnisoLevel() const { return m_anisoLevel; }

    // テクスチャの次元を取得します。
    TextureDimension GetDimension() const { return m_dimension; }

    // テクスチャラップモードを設定します。
    // U,V,W全てに同じラップモードを設定します。
    void SetWrapMode(TextureWrapMode wrapMode);

    // テクスチャU座標のラップモードを設定します。
    void SetWrapModeU(TextureWrapMode wrapMode) { m_wrapModeU = wrapMode; }

    // テクスチャV座標のラップモードを設定します。
    void SetWrapModeV(TextureWrapMode wrapMode) { m_wrapModeV = wrapMode; }

    // テクスチャW座標のラップモードを設定します。
    void SetWrapModeW(TextureWrapMode wrapMode) { m_wrapModeW = wrapMode; }

    // テクスチャU座標のラップモードを取得します。
    TextureWrapMode GetWrapModeU() const { return m_wrapModeU; };

    // テクスチャV座標のラップモードを取得します。
    TextureWrapMode GetWrapModeV() const { return m_wrapModeV; };

    // テクスチャW座標のラップモードを取得します。
    TextureWrapMode GetWrapModeW() const { return m_wrapModeW; };

    // テクスチャのフィルタリングモードを取得します。
    FilterMode GetFilterMode() const { return m_filterMode; }

    // ミップマップのバイアスを取得します。
    float GetMipMapBias() const { return m_mipMapBias; }

    // CPUによるピクセルデータ読み込みが可能な場合は true を返します。
    bool IsReadable() const { return m_isReadable; }

    // テクスチャリソースへのネイティブポインタを取得します。
    virtual void* GetNativeTexturePtr() const = 0;
};

