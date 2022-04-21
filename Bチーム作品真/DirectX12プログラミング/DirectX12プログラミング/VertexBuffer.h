#pragma once
#include "BufferResource.h"


//---------------------------------------------------------------------------------------------------------------------------------------------
// 頂点バッファクラス
// 
//      ・
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class VertexBuffer : public BufferResource
{
private:
    uint32_t m_stride;                  // 頂点ストライド (頂点1個分のバイト数)
    uint32_t m_count;                   // 頂点数
    D3D12_VERTEX_BUFFER_VIEW  m_view;   // 既定の頂点バッファビュー

public:
    // 頂点バッファオブジェクトを生成します。
    //   第1引数 : [in] 頂点1個分のサイズ(単位はバイト)
    //   第2引数 : [in] 頂点数
    //   第3引数 : [in] 初期化データ(不使用の場合は nullptr)
    //   第4引数 : [in] コピー時に使用されるコマンドリスト
    VertexBuffer(uint32_t vertexStride, uint32_t vertexCount, const void* initialData = nullptr, ID3D12GraphicsCommandList* commandList = nullptr);

    // 仮想デストラクタ
    virtual ~VertexBuffer() = default;

    // 頂点1個分のバイト数を取得します。
    uint32_t GetVertexStride() const { return m_stride; }

    // 頂点数を取得します。
    uint32_t GetVertexCount() const { return m_count; }

    // 頂点バッファビューを取得します。
    const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const { return m_view; }
};

