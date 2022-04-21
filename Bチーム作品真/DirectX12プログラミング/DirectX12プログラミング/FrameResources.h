#pragma once
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>

// 前方宣言
class GraphicsEngine;

//---------------------------------------------------------------------------------------------------------------------------------------------
// フレームリソースクラス
// 
//      ・
//      ・
//      ・
//      ・
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class FrameResources
{
private:
    uint32_t                        m_backBufferIndex;      // レンダリング先となるバックバッファのインデックス
    ID3D12Resource*                 m_backBufferResource;   // レンダリング先となるバックバッファ
    ID3D12CommandAllocator*         m_commandAllocator;     // このフレームで使用するコマンドアロケーター
    ID3D12GraphicsCommandList*      m_commandList;          // このフレームで使用するコマンドリスト
    ID3D12Fence*                    m_fence;                // このフレームで使用するフェンス
    HANDLE                          m_fenceEvent;           // このフレームで使用するフェンスイベント
    D3D12_CPU_DESCRIPTOR_HANDLE     m_descHandleForRTV;     // このフレームでレンダーターゲットビュー(RTV)ディスクリプタへのポインタ
    friend class GraphicsEngine;                            // GraphicsEngineクラスは友達

public:
    // コンストラクタ
    FrameResources();

    // デストラクタ
    ~FrameResources() = default;

    // バックバッファを取得します。
    ID3D12Resource* GetBackBuffer() const { return m_backBufferResource; }

    // コマンドアロケーターを取得します。
    ID3D12CommandAllocator* GetCommandAllocator() const { return m_commandAllocator; }

    // コマンドリストを取得します。
    ID3D12GraphicsCommandList* GetCommandList() const { return m_commandList; }

    // フェンスを取得します。
    ID3D12Fence* GetFence() const { return m_fence; }

    // フェンスイベントを取得します。
    HANDLE GetFenceEvent() const { return m_fenceEvent; }

    // レンダーターゲットビューを取得します。
    const D3D12_CPU_DESCRIPTOR_HANDLE& GetCPUDescriptorHandleForRTV() const { return m_descHandleForRTV; }

    // 前回のフレーム処理が完了するまで待機します。
    void WaitForCompletion(ID3D12CommandQueue* commandQueue);
};

