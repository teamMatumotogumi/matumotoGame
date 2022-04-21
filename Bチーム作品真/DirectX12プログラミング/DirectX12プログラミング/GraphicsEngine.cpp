#include "GraphicsEngine.h"
#include "FrameResources.h"
#include "DepthStencil.h"
#include <cstdio>
#include <cassert>


// 静的メンバ変数の実体を宣言
GraphicsEngine* GraphicsEngine::s_singletonInstance = nullptr;


void GraphicsEngine::CreateSingletonInstance(HWND hWnd, uint32_t backBufferWidth, uint32_t backBufferHeight)
{
    assert(!s_singletonInstance);
    s_singletonInstance = new GraphicsEngine();
    s_singletonInstance->Create(hWnd, backBufferWidth, backBufferHeight);
}


void GraphicsEngine::DestroySingletonInstance()
{
    assert(s_singletonInstance);
    delete s_singletonInstance;
    s_singletonInstance = nullptr;
}


GraphicsEngine::GraphicsEngine()
    : m_dxgiSwapChain(nullptr)
    , m_d3d12Device(nullptr)
    , m_defaultCommandQueue(nullptr)
    , m_defaultFence(nullptr)
    , m_defaultFenceEvent(nullptr)
    , m_defaultDepthStencil(nullptr)
    , m_frameResourcesIndex(0)
    , m_descHeapForRTVs(nullptr)
{
    memset(&m_dxgiSwapChainDesc, 0, sizeof(m_dxgiSwapChainDesc));
}


GraphicsEngine::~GraphicsEngine()
{
    CloseHandle(m_defaultFenceEvent);
    m_defaultFence->Release();
    m_defaultDepthStencil->Release();
    m_defaultCommandQueue->Release();      
    m_dxgiSwapChain->Release();            
    m_d3d12Device->Release();              
}


void GraphicsEngine::Create(HWND hWnd, uint32_t backBufferWidth, uint32_t backBufferHeight)
{
    EnableDebugLayer();
    CreateD3D12Device();
    EnableMessageFiltering(m_d3d12Device);
    CreateDefaultCommandQueue(m_d3d12Device);
    m_defaultDepthStencil = new DepthStencil(backBufferWidth, backBufferHeight, DepthStencilFormat::Depth32);
    m_defaultFence = CreateFence(m_d3d12Device, 1);
    CreateSwapChain(m_defaultCommandQueue, hWnd, backBufferWidth, backBufferHeight);
    CreateFrameResourcesList(NumFrameResources);
}


void GraphicsEngine::EnableDebugLayer()
{
    //----------------------------------------------------------------------------------------------------------------------------
    // Direct3D12デバッグの取得
    // 
    //  D3D12GetDebugInterface
    //  (
    //      第1引数 : [入力] インターフェースID
    //      第2引数 : [出力] インターフェースを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //----------------------------------------------------------------------------------------------------------------------------
    ID3D12Debug* d3d12Debug;
    if (FAILED(D3D12GetDebugInterface(IID_ID3D12Debug, (void**)&d3d12Debug)))
    {
        printf("[失敗] Direct3D12デバッグインターフェイスの取得\n");
        assert(0);
    }
    printf("[成功] Direct3D12デバッグインターフェイスの取得 (アドレス: 0x%p)\n", d3d12Debug);

    // D3D12のデバッグ機能を有効化
    d3d12Debug->EnableDebugLayer();

    // 不要になったインターフェイスの解放
    d3d12Debug->Release();
}


void GraphicsEngine::CreateD3D12Device()
{
    // 優先的に使用する機能レベルテーブル
    static const D3D_FEATURE_LEVEL PreferredFeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    // 実際に作成されたD3D12デバイスの機能レベル
    D3D_FEATURE_LEVEL actuallyCreatedFeatureLevel;

    // 機能レベルを変更しながらD3D12デバイス作成が成功するまで繰り返す
    for (D3D_FEATURE_LEVEL featureLevel : PreferredFeatureLevels)
    {
         //----------------------------------------------------------------------------------------------------------------------------
        // D3D12デバイスの作成
        //
        //  D3D12CreateDevice
        //  (
        //      第1引数 : [入力] どのアダプターを使用するか？ (nullptrの場合は既定のアダプタ)
        //      第2引数 : [入力] 機能レベル (ハードウェアが対応していない場合は古いDirect3Dでエミュレートできる)
        //      第3引数 : [入力] インターフェイスID
        //      第4引数 : [出力] インターフェースを受け取る変数のアドレス
        //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
        //  )
        //----------------------------------------------------------------------------------------------------------------------------
        if (SUCCEEDED(D3D12CreateDevice(nullptr, featureLevel, IID_ID3D12Device, (void**)&m_d3d12Device)))
        {
            actuallyCreatedFeatureLevel = featureLevel;
            break;
        }
    }

    if (!m_d3d12Device)
    {
        printf("[失敗] Direct3D12デバイスの作成\n");
        assert(0);
    }
    printf("[成功] Direct3D12デバイスの作成 (アドレス: 0x%p)\n", m_d3d12Device);
}


void GraphicsEngine::EnableMessageFiltering(ID3D12Device* d3d12Device)
{
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // メッセージフィルタリング
    //---------------------------------------------------------------------------------------------------------------------------------------------
    ID3D12InfoQueue* d3d12InfoQueue;
    if (FAILED(d3d12Device->QueryInterface(IID_ID3D12InfoQueue, (void**)&d3d12InfoQueue)))
    {
        printf("[失敗] Direct3D12情報キューインターフェイスの取得\n");
        assert(0);
    }
    printf("[成功] Direct3D12情報キューインターフェイスの取得 (アドレス: 0x%p)\n", d3d12InfoQueue);

    // 拒否メッセージリスト
    static D3D12_MESSAGE_ID DenyMessageIDList[] =
    {
        D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
    };

    // 重大度リスト
    static D3D12_MESSAGE_SEVERITY SeverityList[] =
    {
        D3D12_MESSAGE_SEVERITY_INFO,
    };

    D3D12_INFO_QUEUE_FILTER infoQueueuFilter;
    memset(&infoQueueuFilter, 0, sizeof(infoQueueuFilter));
    infoQueueuFilter.DenyList.NumIDs = _countof(DenyMessageIDList);
    infoQueueuFilter.DenyList.pIDList = DenyMessageIDList;
    infoQueueuFilter.DenyList.NumSeverities = _countof(SeverityList);
    infoQueueuFilter.DenyList.pSeverityList = SeverityList;
    if (FAILED(d3d12InfoQueue->PushStorageFilter(&infoQueueuFilter)))
    {
        printf("[失敗] ストレージフィルタースタックへのプッシュ\n");
        assert(0);
    }
    printf("[成功] ストレージフィルタースタックへのプッシュ\n");

    if (FAILED(d3d12InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE)))
    {
        printf("[失敗] エラー発生時のブレーク設定\n");
        assert(0);
    }
    printf("[成功] エラー発生時のブレーク設定\n");

    // 不要になったインターフェイスの解放
    d3d12InfoQueue->Release();
}



void GraphicsEngine::CreateDefaultCommandQueue(ID3D12Device* d3d12Device)
{
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // コマンドキュー作成に関する詳細情報
    //---------------------------------------------------------------------------------------------------------------------------------------------
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
    memset(&commandQueueDesc, 0, sizeof(commandQueueDesc));
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;           // [入力] タイプは「3Dエンジン」 (「コンピュートエンジン」「コピーエンジン」を含む全てのコマンドが使用できる)
    commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;  // [入力] 優先度：通常
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;           // [入力] フラグ指定なし
    commandQueueDesc.NodeMask = 0;                                    // [入力] 単一アダプタの場合は 0 または 1

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // コマンドキューの作成
    //
    //  ID3D12Device::CreateCommandQueue
    //  (
    //      第1引数 : [入力] コマンドキュー作成に関する詳細情報
    //      第2引数 : [入力] インターフェースID
    //      第3引数 : [出力] インターフェースを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    if (FAILED(d3d12Device->CreateCommandQueue(&commandQueueDesc, IID_ID3D12CommandQueue, (void**)&m_defaultCommandQueue)))
    {
        printf("[失敗] コマンドキューの作成\n");
        assert(0);
    }
    printf("[成功] コマンドキューの作成 (アドレス: 0x%p)\n", m_defaultCommandQueue);
}


void GraphicsEngine::CreateSwapChain(ID3D12CommandQueue* d3d12DirectCommandQueue, HWND hWnd, uint32_t backBufferWidth, uint32_t backBufferHeight)
{
    //----------------------------------------------------------------------------------------------------------------------------
    // DXGIファクトリの作成
    // 
    //  CreateDXGIFactory2
    //  (
    //      第1引数 : [入力] 作成時フラグ
    //      第2引数 : [入力] インターフェースID
    //      第3引数 : [出力] インターフェースを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //----------------------------------------------------------------------------------------------------------------------------
    IDXGIFactory3* dxgiFactory;
    if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_IDXGIFactory3, (void**)&dxgiFactory)))
    {
        printf("[失敗] DXGIファクトリの作成\n");
        assert(0);
    }
    printf("[成功] DXGIファクトリの作成 (アドレス: 0x%p)\n", dxgiFactory);


    //----------------------------------------------------------------------------------------------------------------------------
    // DXGIスワップチェーン作成に関する詳細情報
    //----------------------------------------------------------------------------------------------------------------------------
    memset(&m_dxgiSwapChainDesc, 0, sizeof(m_dxgiSwapChainDesc));
    m_dxgiSwapChainDesc.Width = backBufferWidth;								// バックバッファの横幅 (単位はピクセル)
    m_dxgiSwapChainDesc.Height = backBufferHeight;							    // バックバッファの高さ (単位はピクセル)
    m_dxgiSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                    // バックバッファのピクセルフォーマット
    m_dxgiSwapChainDesc.BufferCount = NumBackBuffers;                           // バックバッファの枚数 (2=ダブルバッファ、3=トリプルバッファ)
    m_dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;          // バックバッファの用途 (今回はレンダーターゲットとして使いたい)
    m_dxgiSwapChainDesc.SampleDesc.Count = 1;                                   // マルチンプリング数
    m_dxgiSwapChainDesc.SampleDesc.Quality = 0;                                 // マルチンプリング品質
    m_dxgiSwapChainDesc.Scaling = DXGI_SCALING_STRETCH;                         // スケーリングモード (今回はPresent()時にターゲットサイズに合わせて伸縮させる)
    m_dxgiSwapChainDesc.Stereo = FALSE;                                         // 3Dディスプレイのステレオモード
    m_dxgiSwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;                // α値の振舞い (今回は指定なし)
    m_dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;             // Present()時の振舞い (フリップ実行後にバックバッファの内容を破棄)
    m_dxgiSwapChainDesc.Flags = 0;                                              // 各種オプションフラグ


    //----------------------------------------------------------------------------------------------------------------------------
    // DXGIスワップチェーンの作成
    // 
    //  IDXGIFactory3::CreateSwapChainForHwnd
    //  (
    //      第1引数 : [入力] Direct3D12の場合はDirectコマンドキュー
    //      第2引数 : [入力] レンダリング先となるウィンドウの識別番号 (ウィンドウハンドル)
    //      第3引数 : [入力] スワップチェーン作成に関する詳細情報
    //      第4引数 : [入力] フルスクリーン選択時の詳細情報 (ウィンドウモードの場合はnullptr)
    //      第5引数 : [入力] IDXGIOutputインターフェイス
    //      第6引数 : [出力] インターフェースを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //----------------------------------------------------------------------------------------------------------------------------
    IDXGISwapChain1* dxgiSwapChain;
    if (FAILED(dxgiFactory->CreateSwapChainForHwnd(d3d12DirectCommandQueue, hWnd, &m_dxgiSwapChainDesc, nullptr, nullptr, &dxgiSwapChain)))
    {
        printf("[失敗] DXGIスワップチェーンの作成\n");
        assert(0);
    }
    printf("[成功] DXGIスワップチェーンの作成 (アドレス: 0x%p)\n", dxgiSwapChain);


    //----------------------------------------------------------------------------------------------------------------------------
    // インターフェイスバージョンの変更
    // 
    //  IDXGISwapChain1::QueryInterface
    //  (
    //      第1引数 : [出力] 変更先のインターフェイス型で宣言されたポインタ変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //----------------------------------------------------------------------------------------------------------------------------
    if (FAILED(dxgiSwapChain->QueryInterface(&m_dxgiSwapChain)))
    {
        printf("[失敗] DXGIスワップチェーンインターフェイス4の取得\n");
        assert(0);
    }
    printf("[成功] DXGIスワップチェーンインターフェイス4の取得 (アドレス: 0x%p)\n", m_dxgiSwapChain);

    // 不要になったインターフェイスの解放
    dxgiSwapChain->Release();
    dxgiFactory->Release();
}



static ID3D12DescriptorHeap* CreateDescriptorHeapForRTV(ID3D12Device* d3d12Device, uint32_t numDescriptors)
{
    // レンダーターゲットビュー用ディスクリプタヒープを作成する為の詳細情報
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
    memset(&heapDesc, 0, sizeof(heapDesc));
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;			// ディスクリプタの種類 (今回はRTV)
    heapDesc.NumDescriptors = numDescriptors;               // ディスクリプタ配列の要素数
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;       // 各種オプションフラグ
    heapDesc.NodeMask = 0;                                  // 単一アダプターの場合は 0 または 1

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // レンダーターゲットビュー用ディスクリプタヒープの作成
    // 
    //  ID3D12Device::CreateDescriptorHeap
    //  (
    //      第1引数 : [入力] ディスクリプタヒープ作成に関する詳細情報
    //      第2引数 : [入力] インターフェースID
    //      第3引数 : [出力] インターフェースを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    ID3D12DescriptorHeap* descriptorHeapForRTVs;
    if (FAILED(d3d12Device->CreateDescriptorHeap(&heapDesc, IID_ID3D12DescriptorHeap, (void**)&descriptorHeapForRTVs)))
    {
        printf("[失敗] レンダーターゲットビュー用ディスクリプタヒープの作成\n");
    }
    printf("[成功] レンダーターゲットビュー用ディスクリプタヒープの作成 (アドレス: 0x%p)\n", descriptorHeapForRTVs);
    return descriptorHeapForRTVs;
}


static ID3D12CommandAllocator* CreateDirectCommadAllocator(ID3D12Device* d3d12Device)
{
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // コマンドアロケーターの作成
    // 
    //  ID3D12Device::CreateCommandAllocator
    //  (
    //      第1引数 : [入力] コマンドリストタイプ (コマンドキューとタイプが同じでなければならない)
    //      第2引数 : [入力] インターフェースID
    //      第3引数 : [出力] インターフェースを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    ID3D12CommandAllocator* d3d12CommandAllocator;
    if (FAILED(d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_ID3D12CommandAllocator, (void**)&d3d12CommandAllocator)))
    {
        printf("[失敗] コマンドアロケーターの作成\n");
        assert(0);
    }
    printf("[成功] コマンドアロケーターの作成 (アドレス: 0x%p)\n", d3d12CommandAllocator);
    return d3d12CommandAllocator;
}


static ID3D12GraphicsCommandList* CreateDirectCommadList(ID3D12Device* d3d12Device, ID3D12CommandAllocator* commandAllocator)
{
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // コマンドリストの作成
    // 
    //  ID3D12Device::CreateCommandList
    //  (
    //      第1引数 : [入力] ノードマスク (単一アダプターの場合は 0 または 1)
    //      第2引数 : [入力] コマンドリストタイプ (第3引数のコマンドアロケーターと同じタイプでなければならない)
    //      第3引数 : [入力] コマンドアロケーター (第2引数のコマンドリストタイプと同じタイプでなければならない)
    //      第4引数 : [入力] 初期設定に使用するパイプラインステート (不使用ならば nullptr)
    //      第5引数 : [入力] インターフェースID
    //      第6引数 : [出力] インターフェースを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    ID3D12GraphicsCommandList* d3d12CommandList;
    if (FAILED(d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_ID3D12GraphicsCommandList, (void**)&d3d12CommandList)))
    {
        printf("[失敗] コマンドリストの作成\n");
        assert(0);
    }
    printf("[成功] コマンドリストの作成 (アドレス: 0x%p)\n", d3d12CommandList);
    return d3d12CommandList;
}


ID3D12Fence* GraphicsEngine::CreateFence(ID3D12Device* d3d12Device, UINT64 initialValue)
{
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // フェンスの作成
    // 
    //  ID3D12Device::CreateFence
    //  (
    //      第1引数 : [入力] フェンスの初期値
    //      第2引数 : [入力] 各種オプションフラグ
    //      第3引数 : [入力] インターフェースID
    //      第4引数 : [出力] インターフェースを受け取る変数のアドレス
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    ID3D12Fence* d3d12Fence;
    if (FAILED(d3d12Device->CreateFence(1, D3D12_FENCE_FLAG_NONE, IID_ID3D12Fence, (void**)&d3d12Fence)))
    {
        printf("[失敗] フェンスの作成\n");
        assert(0);
    }
    printf("[成功] フェンスの作成 (アドレス: 0x%p)\n", d3d12Fence);
    return d3d12Fence;
}


static HANDLE CreateFenceEvent()
{
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // イベントオブジェクトの作成
    // 
    //  CreateEvent
    //  (
    //      第1引数 : [入力] セキュリティ属性を表す SECURITY_ATTRIBUTES 型変数のアドレス。 (nullptrでよい)
    //      第2引数 : [入力] シグナル時に自動でリセットさせる場合は TRUE。手動で行いたい場合は FALSE。
    //      第3引数 : [入力] イベントの初期状態。シグナル状態で開始する場合は TRUE。非シグナル状態で開始する場合は FALSE。
    //      第4引数 : [入力] イベント名 (不要な場合は nullptr)
    //      戻り値 : イベントハンドラ
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    HANDLE hFenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!hFenceEvent)
    {
        printf("[失敗] イベントオブジェクトの作成\n");
        assert(0);
    }
    printf("[成功] イベントオブジェクトの作成 (アドレス: 0x%p)\n", hFenceEvent);
    return hFenceEvent;
}


void GraphicsEngine::CreateFrameResourcesList(uint32_t numFrameResourcesSets)
{
    assert(numFrameResourcesSets <= 8);

    // ディスクリプタヒープの作成 (レンダーターゲットビュー用)
    m_descHeapForRTVs = CreateDescriptorHeapForRTV(m_d3d12Device, m_dxgiSwapChainDesc.BufferCount);

    for (uint32_t i = 0; i < m_dxgiSwapChainDesc.BufferCount; i++)
    {
        //---------------------------------------------------------------------------------------------------------------------------------------------
        // バックバッファを取得する
        // 
        //  IDXGISwapChain::GetBuffer
        //  (
        //      第1引数 : [入力] バックバッファのインデックス値 [0 ～ (バックバッファの枚数-1)]
        //      第2引数 : [入力] インターフェースID
        //      第3引数 : [出力] インターフェースを受け取る変数のアドレス
        //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
        //  )
        //---------------------------------------------------------------------------------------------------------------------------------------------
        ID3D12Resource* d3d12BackBuffer;
        if (FAILED(m_dxgiSwapChain->GetBuffer(i, IID_ID3D12Resource, (void**)&d3d12BackBuffer)))
        {
            printf("[失敗] %d番目のバックバッファの取得\n", i);
            assert(0);
        }
        printf("[成功] %d番目のバックバッファの取得 (アドレス: 0x%p)\n", i, d3d12BackBuffer);

        //---------------------------------------------------------------------------------------------------------------------------------------------
        // レンダーターゲットビュー作成に関する詳細情報
        //---------------------------------------------------------------------------------------------------------------------------------------------
        D3D12_RENDER_TARGET_VIEW_DESC descRTV;
        memset(&descRTV, 0, sizeof(descRTV));
        descRTV.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        descRTV.Format = m_dxgiSwapChainDesc.Format;
        descRTV.Texture2D.MipSlice = 0;
        descRTV.Texture2D.PlaneSlice = 0;

        // ディスクリプタを作成する場所
        D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandleForRTV = m_descHeapForRTVs->GetCPUDescriptorHandleForHeapStart();
        descriptorHandleForRTV.ptr += (SIZE_T)i * m_d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        //---------------------------------------------------------------------------------------------------------------------------------------------
        // レンダーターゲットビューの作成
        // 
        //  ID3D12Device::CreateRenderTargetView
        //  (
        //      第1引数 : [入力] 参照先となるリソース
        //      第2引数 : [入力] レンダーターゲットビュー作成に関する詳細情報
        //      第3引数 : [入力] ディスクリプタを作成する場所
        //       戻り値 : なし
        //  )
        //---------------------------------------------------------------------------------------------------------------------------------------------
        m_d3d12Device->CreateRenderTargetView(d3d12BackBuffer, &descRTV, descriptorHandleForRTV);

        // 不要になったインターフェイスの解放
        d3d12BackBuffer->Release();
    }


    //---------------------------------------------------------------------------------------------------------------------------------------------
    // フレームリソース配列の作成
    //---------------------------------------------------------------------------------------------------------------------------------------------
    m_frameResourcesList.reserve(numFrameResourcesSets);
    for (uint32_t i = 0; i < numFrameResourcesSets; i++)
    {
        // i番目のフレームリソースに関連付けるバックバッファのインデックス
        const uint32_t backBufferIndex = i % m_dxgiSwapChainDesc.BufferCount;

        // backBufferIndex番目のバックバッファの取得
        ID3D12Resource* d3d12BackBuffer;
        if (FAILED(m_dxgiSwapChain->GetBuffer(backBufferIndex, IID_ID3D12Resource, (void**)&d3d12BackBuffer)))
        {
            printf("[失敗] %d番目のバックバッファの取得\n", backBufferIndex);
            assert(0);
        }

        // RTVディスクリプタのCPU仮想アドレス
        D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle;
        cpuDescriptorHandle = m_descHeapForRTVs->GetCPUDescriptorHandleForHeapStart();
        cpuDescriptorHandle.ptr += (SIZE_T)backBufferIndex * m_d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        // フレームリソース情報を格納
        FrameResources* frameResources = new FrameResources();
        frameResources->m_backBufferIndex = backBufferIndex;
        frameResources->m_backBufferResource = d3d12BackBuffer;
        frameResources->m_descHandleForRTV = cpuDescriptorHandle;
        frameResources->m_commandAllocator = CreateDirectCommadAllocator(m_d3d12Device);
        frameResources->m_commandList = CreateDirectCommadList(m_d3d12Device, frameResources->m_commandAllocator);
        frameResources->m_fence = CreateFence(m_d3d12Device, 1);
        frameResources->m_fenceEvent = CreateFenceEvent();
        m_frameResourcesList.push_back(frameResources);
    }
}


void GraphicsEngine::WaitForCompletion()
{
    // フェンスに書き込む値
    const UINT64 completionValue = m_defaultFence->GetCompletedValue() + 1;

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // コマンドキューにシグナルコマンドを追加します。
    // 
    //  ID3D12CommandQueue::Signal
    //  (
    //      第1引数 : [入力] フェンス
    //      第2引数 : [入力] GPUの処理完了時にフェンスに書き込む値
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    if (FAILED(m_defaultCommandQueue->Signal(m_defaultFence, completionValue)))
    {
        assert(0);
    }

    // この時点でフェンスの現在値が完了値に到達していることもあるので調べる。
    if (m_defaultFence->GetCompletedValue() < completionValue)
    {
        // まだ到達していないので完了通知イベントをONにする
        if (FAILED(m_defaultFence->SetEventOnCompletion(completionValue, m_defaultFenceEvent)))
        {
            assert(0);
        }

        // 完了通知をひたすら待つ
        WaitForSingleObject(m_defaultFenceEvent, INFINITE);
    }
}


void GraphicsEngine::Present()
{
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 現在のバックバッファをフロントバッファと交換してディスプレイに画像を転送する。
    // 
    //  IDXGISwapChain4::Present
    //  (
    //      第1引数 : [入力] 垂直帰線消去を待つ回数 (0の場合は待たずに即時転送を開始します)
    //      第2引数 : [入力] フラグ
    //       戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
    //  )
    //---------------------------------------------------------------------------------------------------------------------------------------------
    if (FAILED(m_dxgiSwapChain->Present(0, 0)))
    {
        printf("[失敗] IDXGISwapChain4::Present()\n");
        assert(0);
    }

    // 次のフレームの処理へ
    m_frameResourcesIndex = (m_frameResourcesIndex + 1) % m_frameResourcesList.size();
}

