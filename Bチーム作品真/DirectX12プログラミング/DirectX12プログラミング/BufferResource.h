#pragma once
#include <d3d12.h>
#include <cstdint>
#include "ReferenceCounter.h"


// バッファの種類を表す列挙型
enum class BufferResourceType
{
    Unspecified,
    ConstantBuffer,
    VertexBuffer,
    IndexBuffer,
};


// Map時に指定する範囲型    
struct MapRange
{
    uint32_t subresource;
    size_t begin;
    size_t end;
};


// バッファリソースクラス
class BufferResource : public ReferenceCounter
{
private:
    ID3D12Resource*         m_defaultHeapBuffer;        // デフォルトヒープバッファ
    ID3D12Resource*         m_uploadHeapBuffer;         // アップロードヒープバッファ
    BufferResourceType      m_type;                     // バッファタイプ
    uint64_t                m_byteWidth;                // バッファサイズ (単位はバイト)

protected:
    // コンストラクタ
    BufferResource();

    // 仮想デストラクタ
    virtual ~BufferResource();

    // バッファリソースを作成します。
    //      第1引数 : [in] バッファの種類を識別する列挙型の値
    //      第2引数 : [in] バッファサイズ (単位はバイト)
    //      第3引数 : [in] 初期化データ (不使用の場合は nullptr)
    //      第4引数 : [in] 初期化データのアップロード時に使用されるコマンドリスト
    //       戻り値 :      成否を表すbool型の値
    bool CreateResource(BufferResourceType bufferResourceType, uint64_t byteWidth, const void* initialData = nullptr, ID3D12GraphicsCommandList* commandList = nullptr);

public:
    // バッファサイズ(単位はバイト)を取得します。
    uint64_t GetBufferSize() const { return m_byteWidth; }

    // アップロードヒープバッファに対する書き込み用アドレスを取得します。
    void* Map(const MapRange* mapRange = nullptr);

    // アップロードヒープバッファをアンマップします。
    void Unmap(const MapRange* mapRange = nullptr, ID3D12GraphicsCommandList* commandList = nullptr);

    /// Direct3D12リソースオブジェクトを取得します。
    ID3D12Resource* GetNativeResource() const { return m_defaultHeapBuffer; }

private:
    // アップロードヒープバッファを作成します。
    //      第1引数 : [in] バッファの種類を識別する列挙型の値
    //      第2引数 : [in] バッファサイズ (単位はバイト)
    //      第3引数 : [in] 初期化データ (不使用の場合は nullptr)
    //       戻り値 :      バッファリソース
    ID3D12Resource* CreateUploadHeapBuffer(BufferResourceType bufferResourceType, uint64_t byteWidth, const void* initialData = nullptr);

    // デフォルトヒープバッファを作成します。
    //      第1引数 : [in] バッファの種類を識別する列挙型の値
    //      第2引数 : [in] バッファサイズ (単位はバイト)
    //       戻り値 :      バッファリソース
    ID3D12Resource* CreateDefaultHeapBuffer(BufferResourceType bufferResourceType, uint64_t byteWidth);

    // アップロードヒープバッファの内容をデフォルトヒープバッファにコピーします。
    //      第1引数 : [in] バッファの種類を識別する列挙型の値
    //      第2引数 : [in] コピー時に使用されるコマンドリスト
    void Upload(BufferResourceType bufferResourceType, ID3D12GraphicsCommandList* commandList = nullptr);
};


