#include "SlidePuzzle.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Rect.h"
#include "Vector2.h"
#include "Transform.h"
#include "Keyboard.h"

#include <time.h>


// 2つの変数の値を交換する関数 (テンプレート版)
template<typename T>
void Swap(T& a, T& b)
{
    T c = a;
    a = b;
    b = c;
}


SlidePuzzle::SlidePuzzle(
    HWND hWnd,
    uint32_t gameScreenResolutionWidth,
    uint32_t gameScreenResolutionHeight)
    : m_hWnd(hWnd)
    , m_gameScreenResolutionWidth(gameScreenResolutionWidth)
    , m_gameScreenResolutionHeight(gameScreenResolutionHeight)
{
}

void SlidePuzzle::Start()
{
    // テクスチャ
    Texture2D* panelTexture = Texture2D::FromFile(L"Assets/tensura-1.jpg");

    // パネル1枚分の横幅と高さを計算する。
    panelWidth = panelTexture->GetWidth() / 4;
    panelHeight = panelTexture->GetHeight() / 4;

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            // ゲームオブジェクトの作成
            panels[y][x] = new GameObject();

            // パネルスプライトの作成
            Sprite* panelSprite = Sprite::Create(panelTexture, Rect((float)(x * panelWidth), (float)(y * panelHeight), (float)panelWidth, (float)panelHeight), Vector2(0.0f, 1.0f), 1.0f);

            // SpriteRendererコンポーネントの追加
            SpriteRenderer* spriteRenderer = panels[y][x]->AddComponent<SpriteRenderer>();
            spriteRenderer->SetSprite(panelSprite);

            // 右下のパネルは不可視状態にする
            if (x == 3 && y == 3)
            {
                spriteRenderer->SetEnabled(false);
            }
        }
    }

    // 空きパネルであるゲームオブジェクトを覚えておく
    emptyPanel = panels[3][3];

    // シャッフル前の状態を保存しておく (クリア判定用)
    memcpy(saved, panels, sizeof(saved));


    // 空きパネルの場所を覚えておく
    emptyPanelX = 3;
    emptyPanelY = 3;

    // パネルのシャッフル

    // 方向テーブル
    const int dx[4] = { 0, 1, 0, -1 };
    const int dy[4] = { -1, 0, 1,  0 };

    // ランダムシード(乱数の種)の変更
    srand((unsigned int)time(nullptr));

    for (int i = 0; i < 1000; i++)
    {
        // 0～3 のランダムな整数値を生成する
        const int random = rand() % 4;

        // 空きパネルに隣接するパネルの位置をランダムに決める
        const int adjacentPanelX = emptyPanelX + dx[random];
        const int adjacentPanelY = emptyPanelY + dy[random];

        if (adjacentPanelX >= 0 && adjacentPanelX < 4 &&
            adjacentPanelY >= 0 && adjacentPanelY < 4)
        {
            // 空きパネルと隣接パネルを交換する
            Swap(panels[emptyPanelY][emptyPanelX], panels[adjacentPanelY][adjacentPanelX]);

            // 空きパネルの位置を上書き
            emptyPanelX = adjacentPanelX;
            emptyPanelY = adjacentPanelY;
        }
    }


    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            // Transformコンポーネントの取得
            Transform* transform = panels[y][x]->GetTransform();
            transform->SetLocalPosition(float(x * panelWidth), float((4 * panelHeight) - y * panelHeight), 0.0f);
        }
    }

    // 背景音(BGM)をOPENしておく。
    MCI_OPEN_PARMS openParams;
    openParams.dwCallback = (DWORD)0;
    openParams.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
    openParams.lpstrElementName = "Assets/Audio/BGM/魔王魂 ファンタジー14.wav";
    openParams.lpstrAlias = nullptr;
    openParams.wDeviceID = 0;
    if (mciSendCommand(
        0,                  // 曲を識別する番号 (OPEN時は必ず0)
        MCI_OPEN,           // 今回やりたい事 (曲をOPENしたい！)
        MCI_OPEN_TYPE |     // MCI_OPEN_TYPE_〇〇が有効
        MCI_OPEN_TYPE_ID |  // デバイスをIDで指定する
        MCI_OPEN_ELEMENT,   // 曲をファイル名で指定します
        (DWORD_PTR)&openParams
    ))
    {
        printf("BGMファイルを開けませんでした\n");
    }

    // 背景音(BGM)を再生する
    if (mciSendCommand(
        openParams.wDeviceID,   // 再生したい曲のID
        MCI_PLAY,               // 今回やりたい事 (曲を再生したい！)
        0,                      // 有効なメンバ
        0                       // 再生に関する詳細情報
    ))
    {
        printf("BGMの再生に失敗しました\n");
    }
}

void SlidePuzzle::Update()
{
    // マウスカーソルの位置を取得する。 (ただし、スクリーン座標)
    POINT cursorPosInScreen;
    GetCursorPos(&cursorPosInScreen);

    // 「スクリーン座標」のままではゲームで使えないので「クライアント座標」に変換する。
    POINT cursorPosInClient = cursorPosInScreen;
    ScreenToClient(m_hWnd, &cursorPosInClient);

    // 「クライアント座標」を「ワールド空間座標」に変換する。
    POINT cursorPosInWorld;
    cursorPosInWorld.x = cursorPosInClient.x;
    cursorPosInWorld.y = m_gameScreenResolutionHeight - cursorPosInClient.y;

    // 「ワールド空間座標」を「パネル座標」に変換する。
    POINT clickedPanel;
    clickedPanel.x = cursorPosInWorld.x / panelWidth;       // 左から何枚目のパネル？
    clickedPanel.y = 3 - (cursorPosInWorld.y / panelHeight);// 上から何枚目のパネル？

    // ウィンドウの左上にマウスカーソルの位置を表示する
    char windowText[256];
    sprintf_s(windowText, "スクリーン(%4d, %4d)  クライアント(%4d, %4d)  ワールド(%4d, %4d)  パネル(%2d, %2d)",
        cursorPosInScreen.x, cursorPosInScreen.y, cursorPosInClient.x, cursorPosInClient.y,
        cursorPosInWorld.x, cursorPosInWorld.y, clickedPanel.x, clickedPanel.y);
    SetWindowText(m_hWnd, windowText);

    // パネルの位置が変更されたか？
    bool isPanelChanged = false;

    // マウスの左ボタンがクリックされたら
    if (Keyboard::JustPressed(VK_LBUTTON))
    {
        // パネル上でクリックされたか？
        if (clickedPanel.x >= 0 && clickedPanel.x <= 3 &&
            clickedPanel.y >= 0 && clickedPanel.y <= 3)
        {
            // クリックされたパネルの上下左右に空きパネルがあるか？

            // 1つ上を調べる
            if (clickedPanel.y > 0 && panels[clickedPanel.y - 1][clickedPanel.x] == emptyPanel)
            {
                Swap(panels[clickedPanel.y][clickedPanel.x], panels[emptyPanelY][emptyPanelX]);
                emptyPanelX = clickedPanel.x;
                emptyPanelY = clickedPanel.y;
                isPanelChanged = true;
            }
            // 1つ下を調べる
            else if (clickedPanel.y < 3 && panels[clickedPanel.y + 1][clickedPanel.x] == emptyPanel)
            {
                Swap(panels[clickedPanel.y][clickedPanel.x], panels[emptyPanelY][emptyPanelX]);
                emptyPanelX = clickedPanel.x;
                emptyPanelY = clickedPanel.y;
                isPanelChanged = true;
            }
            // 1つ左を調べる
            else if (clickedPanel.x > 0 && panels[clickedPanel.y][clickedPanel.x - 1] == emptyPanel)
            {
                Swap(panels[clickedPanel.y][clickedPanel.x], panels[emptyPanelY][emptyPanelX]);
                emptyPanelX = clickedPanel.x;
                emptyPanelY = clickedPanel.y;
                isPanelChanged = true;
            }
            // 1つ右を調べる
            else if (clickedPanel.x < 3 && panels[clickedPanel.y][clickedPanel.x + 1] == emptyPanel)
            {
                Swap(panels[clickedPanel.y][clickedPanel.x], panels[emptyPanelY][emptyPanelX]);
                emptyPanelX = clickedPanel.x;
                emptyPanelY = clickedPanel.y;
                isPanelChanged = true;
            }
        }
    }

    // キーボードの'W'が押されたら「空きパネル」と「その1つ下のパネル」を入れ替える
    if (Keyboard::JustPressed('W'))
    {
        if (emptyPanelY < 3)
        {
            Swap(panels[emptyPanelY][emptyPanelX], panels[emptyPanelY + 1][emptyPanelX]);
            emptyPanelY++;
            isPanelChanged = true;
        }
    }

    // キーボードの'A'が押されたら「空きパネル」と「その1つ右のパネル」を入れ替える
    if (Keyboard::JustPressed('A'))
    {
        if (emptyPanelX < 3)
        {
            Swap(panels[emptyPanelY][emptyPanelX], panels[emptyPanelY][emptyPanelX + 1]);
            emptyPanelX++;
            isPanelChanged = true;
        }
    }

    // キーボードの'S'が押されたら「空きパネル」と「その1つ上のパネル」を入れ替える
    if (Keyboard::JustPressed('S'))
    {
        if (emptyPanelY > 0)
        {
            Swap(panels[emptyPanelY][emptyPanelX], panels[emptyPanelY - 1][emptyPanelX]);
            emptyPanelY--;
            isPanelChanged = true;
        }
    }

    // キーボードの'D'が押されたら「空きパネル」と「その1つ左のパネル」を入れ替える
    if (Keyboard::JustPressed('D'))
    {
        if (emptyPanelX > 0)
        {
            Swap(panels[emptyPanelY][emptyPanelX], panels[emptyPanelY][emptyPanelX - 1]);
            emptyPanelX--;
            isPanelChanged = true;
        }
    }

    // パネルの位置が変更されていたら
    if (isPanelChanged)
    {
        // 効果音の再生
        PlaySound("Assets/Audio/SE/魔王魂 効果音 システム44.wav", nullptr, SND_ASYNC | SND_FILENAME);

        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                // Transformコンポーネントの取得
                Transform* transform = panels[y][x]->GetTransform();
                transform->SetLocalPosition(float(x * panelWidth), float((4 * panelHeight) - y * panelHeight), 0.0f);
            }
        }

        // クリア判定
        bool isGameClear = true;
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                if (panels[y][x] != saved[y][x])
                {
                    isGameClear = false;
                    break;
                }
            }
        }

        // ゲームクリア処理
        if (isGameClear)
        {
            MessageBox(nullptr, "クリアおめでとう！", "15パズル", MB_OK);
        }

    }
}

void SlidePuzzle::Render()
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            //panels[y][x]->Update();
            //panels[y][x]->Render();
        }
    }
}

void SlidePuzzle::Stop()
{
}
