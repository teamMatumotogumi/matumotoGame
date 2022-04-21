#pragma once
#include "BufferResource.h"


// インデックスフォーマット列挙型
enum class IndexFormat
{
    UInt16,
    UInt32,
};


// インデックスバッファクラス
class IndexBuffer : public BufferResource
{
private:
    IndexFormat             m_format;
    uint32_t                m_stride;
    uint32_t                m_count;
    D3D12_INDEX_BUFFER_VIEW m_view;

public:
    // インデックスバッファオブジェクトを生成します。
    //   第1引数 : [in] インデックスフォーマット
    //   第2引数 : [in] インデックス数
    //   第3引数 : [in] 初期化データ(不使用の場合は nullptr)
    //   第4引数 : [in] コピー時に使用されるコマンドリスト
    IndexBuffer(IndexFormat indexFormat, uint32_t indexCount, const void* initialData = nullptr, ID3D12GraphicsCommandList* commandList = nullptr);

    // 仮想デストラクタ
    virtual ~IndexBuffer();

    // インデックスフォーマットを取得します。
    IndexFormat GetIndexFormat() const { return m_format; }

    // インデックスフォーマットをDXGI_FORMAT型で取得します。
    DXGI_FORMAT GetIndexFormatAsDXGI() const;

    // インデックス1個分のバイト数を取得します。
    uint32_t GetIndexStride() const { return m_stride; }

    // インデックス数を取得します。
    uint32_t GetIndexCount() const { return m_count; }

    // インデックスバッファビューを取得します。
    const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const { return m_view; }
};

