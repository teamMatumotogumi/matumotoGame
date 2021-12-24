#include<Windows.h>
#include<process.h>
#include<tchar.h>
#include<d3d12.h>
#include <dxgi1_6.h>					// DXGI (Microsoft DirectX Graphics Infrastructureの略) バージョン1.6
#include <d3dcompiler.h>				// シェーダーファイルのコンパイル機能
#include <DirectXMath.h>				// DirectX系の数学ライブラリ

#include<assert.h>
#include<stdio.h>

#pragma comment(lib, "winmm.lib")           // timeGetTime()の為に必要


//---------------------------------------------------------------------------------------------------------------------------------------------
// グローバル変数 (定数)
// 
// 定数の場合は必ず「const」または「constexpr」を付けよう。
//---------------------------------------------------------------------------------------------------------------------------------------------

static const unsigned int GameScreenResolutionWidth = 1920;
static const unsigned int GameScreenResolutionHeight = 1080;



LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // メッセージの種類で分岐
    switch (message)
    {
        // ウィンドウが閉じられようとしている時の処理
    case WM_DESTROY:
    {
        // このアプリを終了させる
        PostQuitMessage(0);
    }
    break;

    // 興味のないメッセージは DefWindowProc関数に丸投げする。
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow) {
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ウィンドウクラスの登録
    // 
    //  ・ウィンドウを作成する前にウィンドウクラスをOS(Windows)に登録しておく必要がある。
    //  ・ウィンドウクラスは「ウィンドウの性質」のこと。
    //  ・RegisterClassEx関数を呼び出すことでウィンドウクラスを登録することができる。
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------
    WNDCLASSEX wcex;
    memset(&wcex, 0, sizeof(wcex));                             // 埋め忘れが怖いので最初に全部0で埋める
    wcex.cbSize = sizeof(wcex);                                 // この変数自身のサイズ(単位はバイト)
    wcex.cbClsExtra = 0;                                        // ウィンドウクラス用の拡張メモリサイズ
    wcex.cbWndExtra = 0;                                        // ウィンドウ用の拡張メモリサイズ
    wcex.style = CS_HREDRAW | CS_VREDRAW;                       // 再描画フラグ
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   // ウィンドウの背景を塗りつぶす為のブラシ
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);              // ウィンドウ上で表示されるカーソル
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);            // アイコン
    wcex.hIconSm = nullptr;                                     // 小アイコン
    wcex.lpszMenuName = nullptr;                                // メニュー名
    wcex.lpszClassName = _T("Bチーム");             // ウィンドウクラス名 (何でもよい)
    wcex.lpfnWndProc = WndProc;                                 // ウィンドウプロシージャ関数のアドレス
    wcex.hInstance = hInstance;                                 // アプリケーションインスタンスを識別する整数値

    if (!RegisterClassEx(&wcex))
    {
        printf("[失敗] ウィンドウクラスの登録\n");
        assert(0);
    }
    printf("[成功] ウィンドウクラスの登録 (ウィンドウクラス名: %s)\n", wcex.lpszClassName);

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ウィンドウスタイル
    // 
    //   ・ウィンドウの外観や機能を決める為の値。
    //   ・ビットフラグなので複数指定することができる。
    // 
    //          +------------------------+-----------------+------------------------------------------+
    //          |     スタイル名         |     16進値      |        意味                              |
    //          +------------------------+-----------------+------------------------------------------+
    //          |   WS_OVERLAPPED        |   0x00000000    |    タイトルバーと境界線を持つ            |
    //          |   WS_CAPTION           |   0x00C00000    |    タイトルバーを持つ                    |
    //          |   WS_SYSMENU           |   0x00080000    |    タイトルバー上にメニューを持つ        |
    //          |   WS_THICKFRAME        |   0x00040000    |    サイズ変更枠を持つ                    |
    //          |   WS_MINIMIZEBOX       |   0x00020000    |    最小化ボタンを持つ                    |
    //          |   WS_MAXIMIZEBOX       |   0x00010000    |    最大化ボタンを持つ                    |
    //          |   WS_OVERLAPPEDWINDOW  |   0x00CF0000    |    上記全ての複合スタイル                |
    //          +------------------------+-----------------+------------------------------------------+
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------
    const unsigned int windowStyle = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);


    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 指定した「クライント領域サイズ」を基に「ウィンドウサイズ」を計算する。
    //
    //    ・クライアント領域は実際にゲーム画面が表示される場所なのでゲーム画面解像度と一致させよう。
    //    ・ウィンドウサイズ = クライアント領域サイズ + サイズ変更枠等
    //    ・AdjustWindowRect関数で自動的に計算してくれる (便利！)
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------
    RECT rect;
    rect.left = 0;
    rect.right = GameScreenResolutionWidth;
    rect.top = 0;
    rect.bottom = GameScreenResolutionHeight;
    AdjustWindowRect(&rect, windowStyle, FALSE);

    // ウィンドウの横幅 = 右辺のX座標 - 左辺のX座標
    const int windowWidth = rect.right - rect.left;

    // ウィンドウの高さ = 下辺のY座標 - 上辺のY座標
    const int windowHeight = rect.bottom - rect.top;


    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ウィンドウをデスクトップ画面の中央に配置したい。
    // 
    // 「スクリーンサイズ」と「ウィンドウサイズ」が分かれば中央に配置(センタリング)することができる。
    //   スクリーンサイズはデスクトップ画面の解像度のことでGetSystemMetrics関数を使って取得することができる。
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------

    // スクリーンサイズを取得
    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // センタリングされたウィンドウの位置 = (スクリーンサイズ - ウィンドウサイズ) / 2
    //const int windowPosX = (screenWidth - windowWidth) / 2;
    //const int windowPosY = (screenHeight - windowHeight) / 2;
    const int windowPosX = 0;
    const int windowPosY = 0;

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ウィンドウの作成
    // 
    //      第1引数 : [in] 拡張ウィンドウスタイル 
    //      第2引数 : [in] ウィンドウクラス名
    //      第3引数 : [in] ウィンドウ名
    //      第4引数 : [in] ウィンドウスタイル
    //      第5引数 : [in] ウィンドウ左上隅の初期位置(X座標)
    //      第6引数 : [in] ウィンドウ左上隅の初期位置(Y座標)
    //      第7引数 : [in] ウィンドウの横幅 (単位はピクセル)
    //      第8引数 : [in] ウィンドウの高さ (単位はピクセル)
    //      第9引数 : [in] 親ウィンドウハンドル (親ウィンドウが無い場合は nullptr)
    //      第1引数 : [in] メニューハンドル (メニューが無い場合は nullptr)
    //     第10引数 : [in] このアプリケーションインスタンスを識別する為のユニークな整数値
    //     第11引数 : [in] WM_CREATEメッセージを処理する際に渡されるユーザー定義の値
    //       戻り値 : ウィンドウハンドル (ウィンドウを識別する為のユニークな整数値)
    //                ただし、作成に失敗した場合は nullptr。
    //---------------------------------------------------------------------------------------------------------------------------------------------
    HWND hWnd = CreateWindowEx(0, wcex.lpszClassName, _T("Bチーム作品"), windowStyle, windowPosX, windowPosY, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        printf("[失敗] ウィンドウの作成\n");
        return false;
    }
    //printf("[成功] ウィンドウの作成 (ウィンドウハンドル:%x)\n", (unsigned int)hWnd);


        // ウィンドウを可視状態に変更する
    ::ShowWindow(hWnd, SW_SHOW);

    // タイマー分解能を最小にする
    timeBeginPeriod(1);

    // ゲーム開始直前の時間を取得する
    DWORD previousFrameTime = timeGetTime();


    MSG msg;
    while (1)
    {
        //---------------------------------------------------------------------------------------------------------------------------------------------
        // メッセージポンプ
        //      Windowsアプリはこれをやらないとウィンドウが固まる。
        //---------------------------------------------------------------------------------------------------------------------------------------------
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // メッセージがあった場合はウィンドウプロシージャに配送する。
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            // アプリ終了メッセージだった場合は while から抜ける
            if (msg.message == WM_QUIT)
                break;
        }


        //---------------------------------------------------------------------------------------------------------------------------------------------
        // 時間を計測してFPSを固定する
        //---------------------------------------------------------------------------------------------------------------------------------------------

        // 1秒間あたりのフレーム数 (目標値)
        static const DWORD TargetFPS = 60;

        // 1フレームあたりの時間 (単位はミリ秒)
        static const DWORD MillisecondsPerFrame = 1000 / TargetFPS;

        // 現在の時間を取得する (単位はミリ秒)
        const DWORD currentTime = timeGetTime();

        // 経過時間 = 現在の時間 - 前回の時間
        const DWORD elapsedTime = currentTime - previousFrameTime;

        // (   経過時間 >= 1フレームあたりの時間)
        if (elapsedTime >= MillisecondsPerFrame)
        {
            // 次フレームの為にフレーム開始時間を更新しておく
            previousFrameTime = currentTime;

            //---------------------------------------------------------------------------------------------------------------------------------------------
            // フレーム処理の開始
            //     ゲーム内の時間を(1/TargetFPS)秒分だけ進める。
            //---------------------------------------------------------------------------------------------------------------------------------------------

            // キーボードの全てのキー入力状態を再検出
            //Keyboard::Update();

            // 進めるべき微小時間⊿t
            const float deltaTime = 1.0f / TargetFPS;

            // シーン更新
           //SceneManager::GetActiveScene()->Update();

            //---------------------------------------------------------------------------------------------------------------------------------------------
            // ゲーム画面をレンダリングする。
            //---------------------------------------------------------------------------------------------------------------------------------------------

            // フレームリソースセットの取得
            //FrameResources* currentFrameResources = GraphicsEngine::Instance().GetCurrentFrameResources();

            // コマンドリストの取得
           // ID3D12GraphicsCommandList* currentCommandList = currentFrameResources->GetCommandList();

            // 「パイプラインステートオブジェクトを設定」コマンドをコマンドリストに追加する。
          //  currentCommandList->SetPipelineState(d3d12PipelineState);

            // 「グラフィックスパイプライン用ルートシグネチャを設定」コマンドをコマンドリストに追加する。
          //  currentCommandList->SetGraphicsRootSignature(d3d12RootSignature);

            // リソースバリア詳細情報
           //D3D12_RESOURCE_BARRIER resourceBarrier;
           //memset(&resourceBarrier, 0, sizeof(resourceBarrier));
           //
           // 「リソースバリア」コマンドをコマンドリストに追加する。
           // 
           //  レンダーターゲット(バックバッファ)のリソース状態を遷移させる。
           // 【プレゼント可能状態】⇒【レンダーターゲット状態】
           //resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
           //resourceBarrier.Transition.pResource = currentFrameResources->GetBackBuffer();
           //resourceBarrier.Transition.Subresource = 0;
           //resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
           //resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
           //resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
           // currentCommandList->ResourceBarrier(1, &resourceBarrier);

            // シーンのレンダリング
            //SceneManager::GetActiveScene()->Render();

            // 「リソースバリア」コマンドをコマンドリストに追加する。
            // 
            //  レンダーターゲット(バックバッファ)のリソース状態を遷移させる。
            // 【レンダーターゲット状態】⇒【プレゼント可能状態】
            //resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            //resourceBarrier.Transition.pResource = currentFrameResources->GetBackBuffer();
            //resourceBarrier.Transition.Subresource = 0;
            //resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
            //resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
            //resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
           // currentCommandList->ResourceBarrier(1, &resourceBarrier);

            // コマンドリストを閉じる。
            //if (FAILED(currentCommandList->Close()))
            //{
            //    assert(0);
            //}

            //---------------------------------------------------------------------------------------------------------------------------------------------
            // コマンドリスト内のコマンドを解釈してGPUコマンドを発行する。
            //---------------------------------------------------------------------------------------------------------------------------------------------

            // コマンドキューを取得
            //ID3D12CommandQueue* commandQueue = GraphicsEngine::Instance().GetDefaultCommandQueue();

            // コマンドリスト配列をコマンドキューに渡してGPUにコマンドを送信する
          //ID3D12CommandList* const commandLists[] =
          //{
          //    currentCommandList,
          //};
          //commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

            // フェンスを使ってGPU側の完了を待つ。
            //currentFrameResources->WaitForCompletion(commandQueue);

            // コマンドアロケーターのリセット
            //ID3D12CommandAllocator* currentCommandAllocator = currentFrameResources->GetCommandAllocator();
            //if (FAILED(currentCommandAllocator->Reset()))
            //{
            //    assert(0);
            //}
            //
            // コマンドリストのリセット
            //if (FAILED(currentCommandList->Reset(currentCommandAllocator, nullptr)))
            //{
            //    assert(0);
            //}

            // 現在のバックバッファをフロントバッファとし、ディスプレイへの転送を開始する。
           // GraphicsEngine::Instance().Present();

            //てｓｔ

        }
    }
}

