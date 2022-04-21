#include "Sprite.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Rect.h"
#include "Vector2.h"
#include <cassert>


Sprite* Sprite::Create(Texture2D* texture, const Rect& rect, const Vector2& pivot, float pixelsPerUnit)
{
    Sprite* sprite = new Sprite(texture, rect, pivot, pixelsPerUnit);
    if (!sprite)
    {
        assert(0);
    }
    return sprite;
}


// 頂点1個分を表す構造体
struct Sprite::SpriteVertex
{
    float x, y;
    float u, v;
};


Sprite::Sprite(Texture2D* texture, const Rect& rect, const Vector2& pivot, float pixelsPerUnit)
    : m_texture(texture)
    , m_textureRect(rect)
    , m_pivot(pivot)
    , m_pixelsPerUnit(pixelsPerUnit)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
{
    assert(m_texture);
    assert(m_pixelsPerUnit > 0.0f);

    // テクスチャの参照カウントを1増やす
    m_texture->AddRef();

    // ポリゴンの横幅と高さ
    const float polyW = rect.width / pixelsPerUnit;
    const float polyH = rect.height / pixelsPerUnit;

    // ポリゴンの矩形
    const float x0 = 0.0f - polyW * pivot.x;
    const float y0 = 0.0f - polyH * pivot.y;
    const float x1 = x0 + polyW;
    const float y1 = y0 + polyH;

    // 頂点配列 (システムメモリ上)
    const Vector2 vertices[] =
    {
        { x0, y0 }, // 左下の頂点
        { x0, y1 }, // 左上の頂点
        { x1, y0 }, // 右下の頂点
        { x1, y1 }, // 右上の頂点
    };

    // インデックス配列 (システムメモリ上)
    static constexpr uint16_t triangles[] =
    {
        0,1,2,      // 左下の三角形
        1,3,2,      // 右上の三角形
    };

    OverrideGeometry(_countof(vertices), vertices, _countof(triangles), triangles);
}


Sprite::~Sprite()
{
    // 頂点バッファを解放
    if (m_vertexBuffer)
        m_vertexBuffer->Release();

    // インデックスバッファを解放
    if (m_indexBuffer)
        m_indexBuffer->Release();

    // テクスチャを解放
    if (m_texture)
        m_texture->Release();
}


void Sprite::OverrideGeometry(uint16_t numVertices, const Vector2 vertices[], uint16_t numTriangles, const uint16_t triangles[])
{
    assert(numVertices > 0);
    assert(vertices);
    assert(numTriangles > 0);
    assert(triangles);

    // 頂点バッファの破棄
    if (m_vertexBuffer)
        m_vertexBuffer->Release();

    // インデックスバッファの破棄
    if (m_indexBuffer)
        m_indexBuffer->Release();

    // 頂点配列から境界矩形を求める
    Vector2 boundingMin(Vector2::PositiveInfinity);
    Vector2 boundingMax(Vector2::NegativeInfinity);
    for (uint16_t i = 0; i < numVertices; i++)
    {
        // 最小値
        if (vertices[i].x < boundingMin.x) boundingMin.x = vertices[i].x;
        if (vertices[i].y < boundingMin.y) boundingMin.y = vertices[i].y;
        // 最大値
        if (vertices[i].x > boundingMax.x) boundingMax.x = vertices[i].x;
        if (vertices[i].y > boundingMax.y) boundingMax.y = vertices[i].y;
    }

    // テクスチャの横幅と高さ (単位はピクセル)
    const float texW = (float)m_texture->GetWidth();
    const float texH = (float)m_texture->GetHeight();

    // 境界サイズ
    const float w = boundingMax.x - boundingMin.x;
    const float h = boundingMax.y - boundingMin.y;

    m_uv.resize(numVertices);
    for (uint16_t i = 0; i < numVertices; i++)
    {
        // 内分比 (鉛直方向は上下逆)
        const float internalRatioX = (vertices[i].x - boundingMin.x) / w;
        const float internalRatioY = 1.0f - ((vertices[i].y - boundingMin.y) / h);

        // UV座標系に変換
        m_uv[i].x = (m_textureRect.x + m_textureRect.width * internalRatioX) / texW;
        m_uv[i].y = (m_textureRect.y + m_textureRect.height * internalRatioY) / texH;
    }

    SpriteVertex* spriteVertices = new SpriteVertex[numVertices];
    for (uint16_t i = 0; i < numVertices; i++)
    {
        spriteVertices[i].x = vertices[i].x;
        spriteVertices[i].y = vertices[i].y;
        spriteVertices[i].u = m_uv[i].x;
        spriteVertices[i].v = m_uv[i].y;
    }

    // 頂点バッファの作成 (ビデオメモリ上)
    m_vertexBuffer = new VertexBuffer(sizeof(SpriteVertex), numVertices, spriteVertices);
    delete[] spriteVertices;

    // インデックスバッファの作成 (ビデオメモリ上)
    m_indexBuffer = new IndexBuffer(IndexFormat::UInt16, numTriangles, triangles);

    // システムメモリ上にも保存しておく
    m_vertices.resize(numVertices);
    memcpy(&m_vertices[0], vertices, sizeof(vertices[0]) * numVertices);

    // システムメモリ上にも保存しておく
    m_triangles.resize(numTriangles);
    memcpy(&m_triangles[0], triangles, sizeof(triangles[0]) * numTriangles);
}


void Sprite::OverrideGeometry(const std::vector<Vector2>& vertices, const std::vector<uint16_t>& triangles)
{
    OverrideGeometry((uint16_t)vertices.size(), &vertices[0], (uint16_t)triangles.size(), &triangles[0]);
}


