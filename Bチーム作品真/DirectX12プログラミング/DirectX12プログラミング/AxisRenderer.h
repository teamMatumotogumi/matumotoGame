#pragma once
#include "Renderer.h"
#include "Color.h"

// 軸タイプ
enum class AxisType
{
    None,   // 軸なし
    Solid,  // 通常
};


// 軸スタイル
class AxisStyle
{
private:
    AxisType    m_type;	            // 軸の種類
    Color	    m_color;	        // 軸の色
    float	    m_length;	        // 軸の長さ
    bool        m_negative;         // 負数側
    friend class AxisRenderer;

public:
    // コンストラクタ
    AxisStyle();

    // コンストラクタ
    AxisStyle(AxisType axisType, const Color& axisColor, float axisLength = 1.0f, bool negative = false);

    // 軸タイプを設定します。
    void SetType(AxisType axisType) { m_type = axisType; }

    // 軸タイプを取得します。
    AxisType GetType() const { return m_type; }

    // 軸の色を設定します。
    void SetColor(const Color& color) { m_color = color; }

    // 軸の色を取得します。
    const Color& GetColor() const { return m_color; }

    // 軸の長さを設定します。
    void SetLength(float length) { m_length = length; }

    // 軸の長さを取得します。
    float GetLength() const { return m_length; }

    // 軸が負側を持つ場合は true を設定します。
    void HasNegativeSide(bool enable) { m_negative = enable; }

    // 軸が負側を持つ場合は true を返します。
    bool HasNegativeSide() const { return m_negative; }
};


// 前方宣言
class VertexBuffer;
class ConstantBuffer;
class PipelineState;

// XYZ軸レンダラー
class AxisRenderer : public Renderer
{
private:
    AxisStyle       m_xStyle;           // X軸のスタイル
    AxisStyle       m_yStyle;           // Y軸のスタイル
    AxisStyle       m_zStyle;           // Z軸のスタイル
    VertexBuffer*   m_vertexBuffer;     // 頂点バッファ
    uint32_t        m_vertexCount;      // 有効な頂点数
    ConstantBuffer* m_constantBuffer;   // 定数バッファ
    PipelineState*  m_pipelineState;    // パイプラインステート
    bool            m_isGeometryDirty;  // ジオメトリを更新する必要がある場合は true

    struct VertexLayout;                // 頂点レイアウト構造体
    struct ConstantBufferLayout;        // 定数バッファレイアウト構造体
    friend class GameObject;            // GameObjectクラスとは友達

private:
    // コンストラクタ
    AxisRenderer();

    // 仮想デストラクタ
    virtual ~AxisRenderer();

    // 必要であれば頂点バッファを更新します。
    void UploadGeometryData();

    // 
    static uint32_t AddAxis(VertexLayout vertices[], const AxisStyle& axisStyle, const Vector3& direction);

    // Component::Render()のオーバーライド
    void Render() override;

public:
    // このデータ型の情報を返します。
    static const TypeInfo& GetTypeInfo();

    // このインスタンスのデータ型の情報を返します。
    virtual const TypeInfo& GetTypeInfoOfInstance() const { return GetTypeInfo(); }

    // X軸のスタイルを設定します。
    void SetXAxisStyle(const AxisStyle& style) { m_xStyle = style; m_isGeometryDirty = true; }

    // X軸のスタイルを取得します。
    const AxisStyle& GetXAxisStyle() const { return m_xStyle; }

    // Y軸のスタイルを設定します。
    void SetYAxisStyle(const AxisStyle& style) { m_yStyle = style; m_isGeometryDirty = true; }

    // Y軸のスタイルを取得します。
    const AxisStyle& GetYAxisStyle() const { return m_yStyle; }

    // Z軸のスタイルを設定します。
    void SetZAxisStyle(const AxisStyle& style) { m_zStyle = style; m_isGeometryDirty = true; }

    // Z軸のスタイルを取得します。
    const AxisStyle& GetZAxisStyle() const { return m_zStyle; }
};


