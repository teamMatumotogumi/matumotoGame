#pragma once

#include<vector>

namespace MatumotoGame
{
	

	// プレイヤーコントローラー
	class GameScene : public Scene
	{
	public:
		static const uint32_t MaxNumPlayers = 1;

	private:
		GameObject* m_sceneRoot;
		Camera* m_mainCameraComponent;
		GameObject* m_object1;
		GameObject* m_object2;

		// ゲームの進行を表す列挙型
		enum  class State
		{
			waitforClick,    // クリック待ち
			controllable,    // プレイヤー操作可能状態

			
		};

	private:
		
		
		friend class Scene;	      		// シーンクラスは友達
		friend class GameObject;		// ゲームオブジェクトクラスは友達

	public:
		// コンストラクタ
		GameScene() = default;

		// 仮想デストラクタ
		virtual ~GameScene() = default;

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
