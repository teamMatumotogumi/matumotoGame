#pragma once
#include "BufferResource.h"


// 定数バッファクラス
class ConstantBuffer : public BufferResource
{
private:
    D3D12_CONSTANT_BUFFER_VIEW_DESC m_defaultCBV;

public:
    // 定数バッファオブジェクトを作成します。
    //   第1引数 : [in] 定数バッファのサイズ (単位はバイト)
    //   第2引数 : [in] 初期化データ (不使用の場合は nullptr)
    //   第3引数 : [in] コピー時に使用されるコマンドリスト
    //
    //   ※注意 : 定数バッファのサイズは256の倍数でなければならない
    ConstantBuffer(uint32_t byteWidth, const void* initialData = nullptr, ID3D12GraphicsCommandList* commandList = nullptr);

    // 仮想デストラクタ
    virtual ~ConstantBuffer() = default;

    // 既定の定数バッファビューを取得します。
    const D3D12_CONSTANT_BUFFER_VIEW_DESC& GetBufferViewDesc() const { return m_defaultCBV; }
};

