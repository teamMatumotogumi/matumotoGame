#pragma once
#include <DirectXMath.h>
#include "Renderer.h"
#include "Color.h"
#include "Vector3.h"


// 前方宣言
class VertexBuffer;


// グリッド線タイプ
enum class GridLineType
{
    None,
    Solid,
};


// グリッド線スタイル
class GridLineStyle
{
public:
    GridLineType	m_type;		// グリッド線の種類
    Color			m_color;	// グリッド線の色

public:
    // コンストラクタ
    GridLineStyle(GridLineType type, const Color& color);
}; 


// グリッド線レンダラー
class GridLinesRenderer : public Renderer
{
private:
    uint32_t		m_majorGridLines;			// 主線の本数
    GridLineStyle	m_majorGridLineStyle;		// 主線のスタイル
    float			m_majorGridLineInterval;	// 主線間の間隔
    uint32_t		m_minorPerMajor;			// 主線間の副グリッド線の本数
    GridLineStyle	m_minorGridLineStyle;		// 副線のスタイル
    bool			m_isGeometryDirty;			// ジオメトリを再アップロードする必要がある場合は true
    VertexBuffer*   m_vertexBuffer;             // 頂点バッファ

public:
    // このデータ型の情報を返します。
    static const TypeInfo& GetTypeInfo();

    // このインスタンスのデータ型の情報を返します。
    virtual const TypeInfo& GetTypeInfoOfInstance() const { return GetTypeInfo(); }

    // 主線の本数を設定します。
    void SetMajorGridLines(uint32_t count) { m_majorGridLines = count; }

    // 主線のスタイルを設定します。
    void SetMajorGridLineStyle(const GridLineStyle& style) { m_majorGridLineStyle = style; }

    // 主線の間隔を設定します。
    void SetMajorGridLineInterval(float interval) { m_majorGridLineInterval = interval; }

    // 主線と主線の間の副線の本数を設定します。
    void SetMinorPerMajor(uint32_t count) { m_minorPerMajor = count; }

    // 副線のスタイルを設定します。
    void SetMinorGridLineStyle(const GridLineStyle& style) { m_minorGridLineStyle = style; }

private:
    struct VertexLayout;          // 頂点のレイアウト構造体
    struct ConstantBufferLayout;  // 定数バッファのレイアウト構造体
    friend class GameObject;      // GameObjectクラスは友達

    // コンストラクタ
    GridLinesRenderer();

    // 仮想デストラクタ
    virtual ~GridLinesRenderer();

    // ジオメトリデータをGPUが処理可能な場所にアップロードします。
    void UploadGeometryData();

    // Component::Render()のオーバーライド
    void Render() override;
};





