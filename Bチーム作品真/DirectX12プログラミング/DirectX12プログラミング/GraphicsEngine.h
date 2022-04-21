
#pragma once
#include <windows.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <cstdint>
#include <wrl.h>
#include <vector>

// 前方宣言
class DepthStencil;
class FrameResources;

//---------------------------------------------------------------------------------------------------------------------------------------------
// グラフィックエンジンクラス
// 
//      ・このクラスはシングルトンパターンで実装されているため、
//        作成関数と破棄関数を明示的に呼び出さなければならない。
//      ・主にDirect3D12の初期化を担当する。
//      ・Directコマンドキューを持つ。
//      ・スワップチェーン情報を持つ。
//      ・フレームリソース配列を持つ。
//      ・バックバッファと互換性のある深度ステンシルを持つ。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class GraphicsEngine
{
    static constexpr int NumBackBuffers = 2;                        // バックバッファ数
    static constexpr int NumFrameResources = NumBackBuffers * 2;    // フレームリソース数

private:
    static GraphicsEngine*          s_singletonInstance;            // シングルトンインスタンス
    DXGI_SWAP_CHAIN_DESC1           m_dxgiSwapChainDesc;            // DXGIスワップチェーン詳細情報
    IDXGISwapChain4*                m_dxgiSwapChain;                // DXGIスワップチェーン
    ID3D12Device*                   m_d3d12Device;                  // D3D12デバイス
    ID3D12CommandQueue*             m_defaultCommandQueue;          // デフォルトコマンドキュー
    ID3D12Fence*                    m_defaultFence;                 // デフォルトフェンス
    HANDLE                          m_defaultFenceEvent;            // デフォルトフェンスイベント
    DepthStencil*                   m_defaultDepthStencil;          // デフォルト深度ステンシル
    std::vector<FrameResources*>    m_frameResourcesList;           // フレームリソース配列
    uint32_t                        m_frameResourcesIndex;          // フレームリソースの現在のインデックス
    ID3D12DescriptorHeap*           m_descHeapForRTVs;              // レンダーターゲットビュー用ディスクリプタヒープ
    friend class Application;                                       // アプリケーションクラスは友達

private:
    // コンストラクタ
    GraphicsEngine();

    // デストラクタ
    ~GraphicsEngine();

    // フレームレンダリングに必要な必要最低限のリソースを作成します。
    void Create(HWND hWnd, uint32_t backBufferWidth, uint32_t backBufferHeight);

    // D3D12デバッグレイヤーを有効化します。
    void EnableDebugLayer();

    // D3D12デバイスを作成します。
    void CreateD3D12Device();

    // メッセージフィルタリングの有効化
    void EnableMessageFiltering(ID3D12Device* d3d12Device);

    // スワップチェーンを作成します。
    void CreateSwapChain(ID3D12CommandQueue* d3d12DirectCommandQueue, HWND hWnd, uint32_t backBufferWidth, uint32_t backBufferHeight);

    // コマンドキューを作成します。 
    void CreateDefaultCommandQueue(ID3D12Device* d3d12Device);

    // フェンスを作成します。
    static ID3D12Fence* CreateFence(ID3D12Device* d3d12Device, UINT64 initialValue);

    // バックバッファごとのフレームリソースを作成します。
    void CreateFrameResourcesList(uint32_t numFrameResourcesSets);

public:
	// シングルトンインスタンスを作成します。
    static void CreateSingletonInstance(HWND hWnd, uint32_t backBufferWidth, uint32_t backBufferHeight);

	// シングルトンインスタンスを破棄します。
    static void DestroySingletonInstance();

	// シングルトンインスタンスを取得します。
    static GraphicsEngine& Instance() { return *s_singletonInstance; }

    // DXGIスワップチェーンを取得します。
    const DXGI_SWAP_CHAIN_DESC1& GetSwapChainDesc() const { return m_dxgiSwapChainDesc; }

    // D3D12デバイスを取得します。
    ID3D12Device* GetD3D12Device() const { return m_d3d12Device; }

    // デフォルトコマンドキューを取得します。
    ID3D12CommandQueue* GetDefaultCommandQueue() const { return m_defaultCommandQueue; }

    // 深度ステンシルオブジェクトを取得します。
    DepthStencil* GetDepthStencil() const { return m_defaultDepthStencil; }

    // 現在のフレームリソースセットを取得します。
    FrameResources* GetCurrentFrameResources() const { return m_frameResourcesList[m_frameResourcesIndex]; }

    // GPU側の処理が完了するまで待機します。
    void WaitForCompletion();

    // バックバッファへのレンダリングを終了してフロントバッファに指定します。
    // (これによりディスプレイへの転送が開始されます)
    void Present();
};



