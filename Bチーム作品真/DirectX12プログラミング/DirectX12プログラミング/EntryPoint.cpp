
#include"GameScene.h"
#include"Save.h"
//---------------------------------------------------------------------------------------------------------------------------------------------
// 「ヘッダーファイル」だけでは関数を呼び出せないので「ライブラリファイル」をリンクする必要がある
//---------------------------------------------------------------------------------------------------------------------------------------------
#pragma comment(lib, "dxgi.lib")            // DXGIの為に必要
#pragma comment(lib, "d3d12.lib")           // DirectX12に含まれる「DirectXGraphics」コンポーネントの為に必要
#pragma comment(lib, "dxguid.lib")          // 各種インターフェースIDの為に必要
#pragma comment(lib, "d3dcompiler.lib")     // シェーダーをコンパイルする為に必要 (fxc)
#pragma comment(lib, "winmm.lib")           // timeGetTime()の為に必要

//#pragma comment(lib, "./External/Library/DirectXShaderCompiler/x86-64bit/dxcompiler.lib") // シェーダーをコンパイルする為に必要 (dxc)


#if defined(_WIN64)

#if defined(_DEBUG)
#pragma comment(lib, "./External/Library/DirectXTex/x86-64bit/DirectXTex-d.lib")       // DirectXTex (x64 | Debug)
#else
#pragma comment(lib, "./External/Library/DirectXTex/x86-64bit/DirectXTex.lib")         // DirectXTex (x64 | Release)
#endif

#else

#if defined(_DEBUG)
#pragma comment(lib, "./External/Library/DirectXTex/x86-32bit/DirectXTex-d.lib")       // DirectXTex (x86 | Debug)
#else
#pragma comment(lib, "./External/Library/DirectXTex/x86-32bit/DirectXTex.lib")         // DirectXTex (x86 | Release)
#endif

#endif



//---------------------------------------------------------------------------------------------------------------------------------------------
// using directive (using指令 : 主に名前空間の省略に使われる)
//---------------------------------------------------------------------------------------------------------------------------------------------

using namespace DirectX;


//---------------------------------------------------------------------------------------------------------------------------------------------
// using declaration (using宣言 : 主に型名の省略に使われる)
//---------------------------------------------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------------------------------------------
// エイリアステンプレート (using宣言のテンプレート版)
//---------------------------------------------------------------------------------------------------------------------------------------------

template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;



//---------------------------------------------------------------------------------------------------------------------------------------------
// グローバル変数
//---------------------------------------------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------------------------------------------
// グローバル変数 (定数)
// 
// 定数の場合は必ず「const」または「constexpr」を付けよう。
//---------------------------------------------------------------------------------------------------------------------------------------------

static const unsigned int GameScreenResolutionWidth = 1920;
static const unsigned int GameScreenResolutionHeight = 1080;




//---------------------------------------------------------------------------------------------------------------------------------------------
// ウィンドウプロシージャ
// 
//    ・Windowsアプリケーション特有の関数なのでDirectXやゲームとは特に関係がない。
//    ・ウィンドウ上で発生したイベントを処理する為の関数。
//    ・プロシージャは日本語で「手続き」の意味。
//    ・自分で呼び出すのではなくOS(Windows)が呼び出すのでコールバック(呼び戻し)関数とも言われる。
// 
//    第1引数 : [in] 今回のメッセージが発生したウィンドウ番号
//    第2引数 : [in] 今回のメッセージの種類を表す値
//    第3引数 : [in] 付随する情報その1 (メッセージの種類によって異なる)
//    第4引数 : [in] 付随する情報その2 (メッセージの種類によって異なる)
//     戻り値 : 自分でメッセージを処理した場合は 0 を返さなければならない。
//              DefWindowProc関数に処理を丸投げした場合は、その関数の戻り値を返さなければならない。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
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



//---------------------------------------------------------------------------------------------------------------------------------------------
// Windowsアプリケーションのエントリーポイント関数
// 
//    ・アプリを起動した際にOS(Windows)はこのWinMain関数を最初に呼び出すことになっている。
//    ・エントリーポイントは日本語で「侵入地点」「入り口」を意味する。
// 
//    第1引数 : [in] 今回起動したこのアプリケーションの識別番号
//    第2引数 : [in] 前回起動した同じ種類のアプリケーションの識別番号
//    第3引数 : [in] コマンドライン引数と呼ばれている文字列
//    第4引数 : [in] ウィンドウの初期表示状態(最小化、最大化、非表示 etc...)を表す数値
//     戻り値 : 成否を表す整数値 (0未満の場合は失敗、0以上の場合は成功)
//
//---------------------------------------------------------------------------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
    // 未使用の仮引数たち (警告の抑制)
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpszCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 開発時に便利なコンソールウィンドウの作成
    // 
    //   ・AllocConsole関数でコンソールウィンドウを作成することができる。
    //   ・printf関数が使える！
    //   ・不要になったらFreeConsole関数で解放しよう。
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------

    // コンソールウィンドウの作成
    AllocConsole();

    // コンソールウィンドウのウィンドウハンドルを取得
    HWND hConsoleWnd = ::GetConsoleWindow();

    // コンソールウィンドウのシステムメニューハンドルを取得
    HMENU hConsoleSysMenu = ::GetSystemMenu(hConsoleWnd, FALSE);

    // コンソールウィンドウの「閉じるボタン」を押すとアプリが強制終了して正しく終了処理が行われない。
    // それを防ぐ為にシステムメニューを無効化しておく。
    RemoveMenu(hConsoleSysMenu, SC_CLOSE, MF_BYCOMMAND);

    // 標準入出力をコンソールウィンドウに再設定
    FILE* fp = nullptr;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);

    

    PlayData data;
    data.SetPath("Assets/PlayData.txt");
    //data.Save(100);
    printf("%d", data.Load());
    



    //---------------------------------------------------------------------------------------------------------------------------------------------
    // COMシステムの初期化
    // 
    //   ・DirectXはCOM方式で実装されているのであらかじめ初期化関数を呼び出しておく必要がある。
    //   ・COMは「Component Object Model」の略。
    //   ・アプリケーション終了時にはCoUninitialize関数で解放しよう。
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------

    if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
    {
        printf("[失敗] COMの初期化\n");
        return -1;
    }
    printf("[成功] COMの初期化\n");


    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ロケールの設定
    // 
    //   ・対象としている地域を設定する。
    //   ・この設定は文字列処理関数などに影響を与える。
    //   ・とりあえず日本語(Japanese)に設定しておけばOK。
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------

    setlocale(LC_ALL, "Japanese");


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
    wcex.lpszClassName = _T("プログラマー専攻1年");             // ウィンドウクラス名 (何でもよい)
    wcex.lpfnWndProc = WndProc;                                 // ウィンドウプロシージャ関数のアドレス
    wcex.hInstance = hInstance;                                 // アプリケーションインスタンスを識別する整数値

    if ( ! RegisterClassEx(&wcex) )
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
    HWND hWnd = CreateWindowEx(0, wcex.lpszClassName, _T("チーム制作"), windowStyle, windowPosX, windowPosY, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		printf("[失敗] ウィンドウの作成\n");
		return false;
	}
	printf("[成功] ウィンドウの作成 (ウィンドウハンドル:%p)\n", hWnd);


    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ここまでの主要な情報を表示してみる (開発時のみ)
    //---------------------------------------------------------------------------------------------------------------------------------------------
    printf("\n");
	printf("スクリーンサイズ       : %d×%d\n", screenWidth, screenHeight);
	printf("クライアント領域サイズ : %d×%d\n", GameScreenResolutionWidth, GameScreenResolutionHeight);
	printf("ウィンドウサイズ       : %d×%d\n", windowWidth, windowHeight);
	printf("ゲーム画面解像度       : %d×%d\n", GameScreenResolutionWidth, GameScreenResolutionHeight);
    printf("\n");

#if 0
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // PIX on Windows (DirectXグラフィックスアナライザー)
    //---------------------------------------------------------------------------------------------------------------------------------------------

    StartupPIXonWindows();

#endif


    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 入力システムの初期化
    //---------------------------------------------------------------------------------------------------------------------------------------------
    Keyboard::Initialize();


    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 2D/3Dグラフィックスエンジンの初期化
    //---------------------------------------------------------------------------------------------------------------------------------------------
    GraphicsEngine::CreateSingletonInstance(hWnd, GameScreenResolutionWidth, GameScreenResolutionHeight);

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // プログラマブルシェーダーの作成
    //---------------------------------------------------------------------------------------------------------------------------------------------

    // 頂点シェーダーの作成 (頂点シェーダープロファイル 5.1 でコンパイル)
    ShaderBytecode* vertexShader = new ShaderBytecode(L"Assets/Shader/SpriteRendererVS.hlsl", "vs_5_1", "main");

    // ピクセルシェーダーの作成 (ピクセルシェーダープロファイル 5.1 でコンパイル)
    ShaderBytecode* pixelShader = new ShaderBytecode(L"Assets/Shader/SpriteRendererPS.hlsl", "ps_5_1", "main");

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ルートシグネチャの作成
    //---------------------------------------------------------------------------------------------------------------------------------------------
    static const DescriptorRange descriptorRange[] =
    {
        DescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND),
    };

    ID3D12RootSignature* d3d12RootSignature;

    RootSignatureBuilder rootSignatureBuilder;
    rootSignatureBuilder.Begin();
    rootSignatureBuilder.AddRootParameter(RootParameterCBV(0, 0, D3D12_SHADER_VISIBILITY_VERTEX));
    rootSignatureBuilder.AddRootParameter(RootParameterCBV(1, 0, D3D12_SHADER_VISIBILITY_VERTEX));
    rootSignatureBuilder.AddRootParameter(RootParameterCBV(2, 0, D3D12_SHADER_VISIBILITY_VERTEX));
    rootSignatureBuilder.AddRootParameter(RootParameterDescriptorTable(_countof(descriptorRange), descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL));
    rootSignatureBuilder.AddStaticSampler(StaticSampler(0));
    rootSignatureBuilder.End(&d3d12RootSignature);

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 入力レイアウト (英: Input Layout)
    // 
    //  詳細は D3D12_INPUT_ELEMENT_DESC構造体を参照。
    //  https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
    // 
    // 左から順に、
    //      SemanticName         : セマンティック名。任意の識別名を使用できる。 ただし、"SV_"で始まる識別名は予約されているので使用不可。
    //      SemanticIndex        : セマンティック番号。セマンティック名の末尾に追加される数字。
    //      Format               : データフォーマット。
    //      InputSlot            : 入力スロット番号。 このデータを含む頂点バッファのインデックス。
    //      AlignedByteOffset    : 頂点の先頭からこのデータまでのオフセット(単位はバイト)。 D3D12_APPEND_ALIGNED_ELEMENT を指定した場合は自動で計算される。
    //      InputSlotClass       : 入力スロットクラス。
    //      InstanceDataStepRate : 
    //  
    //---------------------------------------------------------------------------------------------------------------------------------------------
    static const D3D12_INPUT_ELEMENT_DESC InputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,    0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0,  8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // グラフィックスパイプラインステート (英: Graphics Pipeline State)
    //---------------------------------------------------------------------------------------------------------------------------------------------
    ID3D12PipelineState* d3d12PipelineState;

    PipelineStateBuilder pipelineStateBuilder;
    pipelineStateBuilder.Begin(false);
    {
        pipelineStateBuilder.SetRootSignature(d3d12RootSignature);
        pipelineStateBuilder.IASetInputElementDescs(_countof(InputElementDescs), InputElementDescs);
        pipelineStateBuilder.VSSetShader(vertexShader);
        pipelineStateBuilder.PSSetShader(pixelShader);
        pipelineStateBuilder.BSSetAlphaToCoverageEnable(true);
        pipelineStateBuilder.BSSetRenderTargetBlend(0, RenderTargetBlend::AlphaBlend);
        pipelineStateBuilder.OMSetNumRenderTargets(1);
        pipelineStateBuilder.OMSetRenderTargetFormat(0, RenderTargetFormat::R8G8B8A8_UNorm);
        pipelineStateBuilder.OMSetDepthStencilFormat(DepthStencilFormat::Depth32);
    }
    pipelineStateBuilder.End(&d3d12PipelineState);

    //---------------------------------------------------------------------------------------------------------------------------------------------
    // 
    //---------------------------------------------------------------------------------------------------------------------------------------------
    // ぷよぷよ「メイン画面」の作成
    MatumotoGame::GameScene* A = new MatumotoGame:: GameScene();

    // ぷよぷよ「メイン画面」をアクティブなシーンとして設定する
    SceneManager::SetActiveScene(A);

    // ぷよぷよ「メイン画面」のアセットをロードする
    A->LoadAssets();
    
    
    // ウィンドウを可視状態に変更する
    ::ShowWindow(hWnd, SW_SHOW);

    // タイマー分解能を最小にする
    timeBeginPeriod(1);

    // ゲーム開始直前の時間を取得する
    DWORD previousFrameTime = timeGetTime();



    // ゲームループ (ゲーム用に改良したメッセージループ)
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
            Keyboard::Update();

            // 進めるべき微小時間⊿t
            const float deltaTime = 1.0f / TargetFPS;

            // シーン更新
            if (SceneManager::GetActiveScene())
            {
                SceneManager::GetActiveScene()->Update();
            }

            //---------------------------------------------------------------------------------------------------------------------------------------------
            // ゲーム画面をレンダリングする。
            //---------------------------------------------------------------------------------------------------------------------------------------------

            // フレームリソースセットの取得
            FrameResources* currentFrameResources = GraphicsEngine::Instance().GetCurrentFrameResources();

            // コマンドリストの取得
            ID3D12GraphicsCommandList* currentCommandList = currentFrameResources->GetCommandList();

            // 「パイプラインステートオブジェクトを設定」コマンドをコマンドリストに追加する。
            currentCommandList->SetPipelineState(d3d12PipelineState);

            // 「グラフィックスパイプライン用ルートシグネチャを設定」コマンドをコマンドリストに追加する。
            currentCommandList->SetGraphicsRootSignature(d3d12RootSignature);

            // リソースバリア詳細情報
            D3D12_RESOURCE_BARRIER resourceBarrier;
            memset(&resourceBarrier, 0, sizeof(resourceBarrier));

            // 「リソースバリア」コマンドをコマンドリストに追加する。
            // 
            //  レンダーターゲット(バックバッファ)のリソース状態を遷移させる。
            // 【プレゼント可能状態】⇒【レンダーターゲット状態】
            resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            resourceBarrier.Transition.pResource = currentFrameResources->GetBackBuffer();
            resourceBarrier.Transition.Subresource = 0;
            resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
            resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
            resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            currentCommandList->ResourceBarrier(1, &resourceBarrier);

            // シーンのレンダリング
            if (SceneManager::GetActiveScene())
            {
                SceneManager::GetActiveScene()->Render();
            }

            // 「リソースバリア」コマンドをコマンドリストに追加する。
            // 
            //  レンダーターゲット(バックバッファ)のリソース状態を遷移させる。
            // 【レンダーターゲット状態】⇒【プレゼント可能状態】
            resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            resourceBarrier.Transition.pResource = currentFrameResources->GetBackBuffer();
            resourceBarrier.Transition.Subresource = 0;
            resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
            resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
            resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            currentCommandList->ResourceBarrier(1, &resourceBarrier);

            // コマンドリストを閉じる。
            if (FAILED(currentCommandList->Close()))
            {
                assert(0);
            }

            //---------------------------------------------------------------------------------------------------------------------------------------------
            // コマンドリスト内のコマンドを解釈してGPUコマンドを発行する。
            //---------------------------------------------------------------------------------------------------------------------------------------------

            // コマンドキューを取得
            ID3D12CommandQueue* commandQueue = GraphicsEngine::Instance().GetDefaultCommandQueue();

            // コマンドリスト配列をコマンドキューに渡してGPUにコマンドを送信する
            ID3D12CommandList* const commandLists[] =
            {
                currentCommandList,
            };
            commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

            // フェンスを使ってGPU側の完了を待つ。
            currentFrameResources->WaitForCompletion(commandQueue);

            // コマンドアロケーターのリセット
            ID3D12CommandAllocator* currentCommandAllocator = currentFrameResources->GetCommandAllocator();
            if (FAILED(currentCommandAllocator->Reset()))
            {
                assert(0);
            }

            // コマンドリストのリセット
            if (FAILED(currentCommandList->Reset(currentCommandAllocator, nullptr)))
            {
                assert(0);
            }

            // 現在のバックバッファをフロントバッファとし、ディスプレイへの転送を開始する。
            GraphicsEngine::Instance().Present();
        }
    }



    // GPU処理の完了を待つ
    GraphicsEngine::Instance().WaitForCompletion();

    // グラフィックスリソースの解放
    d3d12PipelineState->Release();
    d3d12RootSignature->Release();
    vertexShader->Release();
    pixelShader->Release();
    GraphicsEngine::DestroySingletonInstance();

    // タイマー分解能の復帰
    timeEndPeriod(1);

    // COMの終了処理
    CoUninitialize();

    // コンソールウィンドウの解放
    FreeConsole();

    return 0;
}

