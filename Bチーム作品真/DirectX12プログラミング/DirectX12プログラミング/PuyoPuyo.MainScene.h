#pragma once
#include "Scene.h"
#include <vector>

// 前方宣言
class Texture2D;

namespace PuyoPuyo
{
	// 前方宣言
	class PlayerController;

	//---------------------------------------------------------------------------------------------------------------------------------------------
	// ぷよぷよの「メイン画面」シーン
	//
	//		・現状2人プレイにしか対応していない。
	// 
	//---------------------------------------------------------------------------------------------------------------------------------------------
	class MainScene : public Scene
	{
	public:
		static const uint32_t MaxNumPlayers = 2;	// 最大プレイ人数

	private:
		GameObject* m_sceneRoot;								// シーン内のルートゲームオブジェクト
		std::vector<PlayerController*> m_playerControllers;		// プレイヤー配列

	public:
		// コンストラクタ
		MainScene();

		// Scene::LoadAssets()をオーバーライド
		void LoadAssets() override;

		// Scene::Update()をオーバーライド
		void Update() override;

		// Scene::Render()をオーバーライド
		void Render() override;

		// Scene::UnloadAssets()をオーバーライド
		void UnloadAssets() override;

	private:
		// カメラを作成します。
		void CreateMainCamera(Transform* parent);

		// 背景を作成します。
		void CreateBackground(Transform* parent);
	};
}

