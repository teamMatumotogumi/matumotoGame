#pragma once
#include <windows.h>
#include <cstdint>

// 前方宣言
class GameObject;

class SlidePuzzle
{
private:
	// メンバ変数

	HWND m_hWnd;

	// ゲーム画面解像度
	uint32_t m_gameScreenResolutionWidth;
	uint32_t m_gameScreenResolutionHeight;

	// パネルサイズ
	uint32_t panelWidth;
	uint32_t panelHeight;

	// パネル用のゲームオブジェクト (16枚)
	GameObject* panels[4][4];
	GameObject* saved[4][4];

	// 空きパネル
	GameObject* emptyPanel;
	int emptyPanelX;
	int emptyPanelY;

public:
	// コンストラクタ
	SlidePuzzle(HWND hWnd, uint32_t gameScreenResolutionWidth, uint32_t gameScreenResolutionHeight);

	// ゲーム開始時に1度だけ呼び出されます。
	void Start();

	// 毎フレーム呼び出されます。
	void Update();

	// 毎フレーム呼び出されます。
	void Render();

	// ゲーム終了時に1度だけ呼び出されます。
	void Stop();
};

