#pragma once
#include "Object.h"
#include "Texture2D.h"
#include <d3d12.h>
#include <vector>

// 前方宣言
class VertexBuffer;
class IndexBuffer;
class Rect;
class Vector2;

//---------------------------------------------------------------------------------------------------------------------------------------------
// 2Dスプライトクラス
// 
//      ・主に2D開発時に利用されるクラス。
//      ・2Dスプライト生成時にサイズとテクスチャを決定する。
//      ・2Dスプライト生成後にサイズやテクスチャを変更することはできない。
//        
//---------------------------------------------------------------------------------------------------------------------------------------------
class Sprite : public Object
{
private:
    std::string             m_name;             // スプライト名
    Texture2D*              m_texture;          // このスプライトで使用するテクスチャ
    Rect                    m_textureRect;      // このスプライトにマッピングされるテクスチャ上の矩形領域
    Vector2                 m_pivot;            // ピボット
    float                   m_pixelsPerUnit;    // モデル空間での1単位あたりのピクセル数
    std::vector<uint16_t>   m_triangles;        // 頂点インデックス配列
    std::vector<Vector2>    m_uv;               // テクスチャ座標配列
    std::vector<Vector2>    m_vertices;         // 頂点配列
    VertexBuffer*           m_vertexBuffer;     // 頂点バッファ
    IndexBuffer*            m_indexBuffer;      // インデックスバッファ
    struct SpriteVertex;                        // スプライト用頂点レイアウト構造体

private:
    // コンストラクタ
    Sprite(Texture2D* texture, const Rect& rect, const Vector2& pivot, float pixelsPerUnit);

    // 仮想デストラクタ
    virtual ~Sprite() override;

public:
    // 新規スプライトを作成します。
    static Sprite* Create(Texture2D* texture, const Rect& rect, const Vector2& pivot, float pixelsPerUnit);

    // このオブジェクトの識別名を設定します。
    void SetName(const std::string& name) override { m_name = name; }

    // このオブジェクトの識別名を取得します。
    const std::string& GetName() const override { return m_name; }

    // このスプライトで使用するテクスチャを取得します
    Texture2D* GetTexture() const { return m_texture; }

    // ピボットを取得します。
    const Vector2& GetPivot() const { return m_pivot; }

    // モデル空間での1単位あたりのピクセル数
    float GetPixelsPerUnit() const { return m_pixelsPerUnit; }

    // 頂点位置配列を取得します。
    const std::vector<Vector2>& GetVertices() const { return m_vertices; }

    // 頂点インデックス配列を取得します。
    const std::vector<uint16_t>& GetTriangles() const { return m_triangles; }

    // テクスチャ座標配列を取得します。
    const std::vector<Vector2>& GetUV() const { return m_uv; }

    // 既存のスプライトメッシュを上書き作成します。
    void OverrideGeometry(uint16_t numVertices, const Vector2 vertices[], uint16_t numTriangles, const uint16_t triangles[]);

    // 既存のスプライトメッシュを上書き作成します。
    void OverrideGeometry(const std::vector<Vector2>& vertices, const std::vector<uint16_t>& triangles);

    // 頂点バッファを取得します
    VertexBuffer* GetVertexBuffer() const { return m_vertexBuffer; }

    // インデックスバッファを取得します
    IndexBuffer* GetIndexBuffer() const { return m_indexBuffer; }
};

