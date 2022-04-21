#include "PuyoPuyo.MainScene.h"
#include "PuyoPuyo.PlayerController.h"

namespace PuyoPuyo
{
    MainScene::MainScene()
        : m_sceneRoot(nullptr)
    {
    }

    void MainScene::LoadAssets()
	{
        // シーンルートのゲームオブジェクトを作成
        m_sceneRoot = new GameObject("シーンルート");

        // メインカメラを作成
        CreateMainCamera(m_sceneRoot->GetTransform());

        // 背景を作成
        CreateBackground(m_sceneRoot->GetTransform());

        // 1Pの追加
        GameObject* player1 = new GameObject("1P");
        {
            PlayerController* playerController = player1->AddComponent<PlayerController>();
            playerController->Create(PlayerIndex::One, m_sceneRoot->GetTransform());
            m_playerControllers.push_back(playerController);
        }

        // 2Pの追加
        GameObject* player2 = new GameObject("2P");
        {
            PlayerController* playerController = player2->AddComponent<PlayerController>();
            playerController->Create(PlayerIndex::Two, m_sceneRoot->GetTransform());
            m_playerControllers.push_back(playerController);
        }

        // 最大プレイ人数を超えていたらエラー
        if (m_playerControllers.size() > MaxNumPlayers)
        {
            assert(0);
        }

        // 背景音(BGM)の再生
        //PlaySound("Assets/PuyoPuyo/BGM/bgm_es38_heppoko.wav", nullptr, SND_ASYNC | SND_FILENAME | SND_LOOP);
        System::Instance().PlaySharedBGM(System::BackgroundMusicID::es38_heppoko);
    }


    void MainScene::CreateMainCamera(Transform* parent)
    {
        // メインカメラ用のゲームオブジェクトを作成
        GameObject* mainCamera = new GameObject("メインカメラ");

        // カメラコンポーネントを追加する
        Camera* cameraComponent = mainCamera->AddComponent<Camera>();
        cameraComponent->SetOrthographic(true);
        cameraComponent->SetOrthographicSize(1080 / 2);
        cameraComponent->SetAspect(1920 / 1080.0f);
        cameraComponent->SetNearClipPlane(0.3f);
        cameraComponent->SetFarClipPlane(1000.0f);
        cameraComponent->SetClearFlags(CameraClearFlags::SolidColor);
        cameraComponent->SetBackgroundColor(Color::CornflowerBlue);
        cameraComponent->SetNormalizedViewportRect(Rect(0.0f, 0.0f, 1.0f, 1.0f));

        // 位置の変更
        Transform* transform = mainCamera->GetTransform();
        transform->SetParent(parent);
        transform->SetLocalPosition(1920 / 2, 1080 / 2, -10);
    }


    void MainScene::CreateBackground(Transform* parent)
    {
        // 背景上部
        Texture2D* arenaTopTexture = Texture2D::FromFile(L"Assets/PuyoPuyo/Textures/ingame_bg/bg_0001.tzip/stg_puzzlearena_night01_bc3.png");
        GameObject::CreateWithSprite("背景上部", arenaTopTexture, Rect(40, 40, 1920, 1024 - 40), Vector2(0.0f, 0.0f), 1.0f, Vector3(0, 96, 100), parent);

        // 背景下部
        Texture2D* arenaBottomTexture = Texture2D::FromFile(L"Assets/PuyoPuyo/Textures/ingame_bg/bg_0001.tzip/stg_puzzlearena_night02_bc3.png");
        GameObject::CreateWithSprite("背景下部", arenaBottomTexture, Rect(40, 0, 1920, 96), Vector2(0.0f, 0.0f), 1.0f, Vector3(0, 0, 100), parent);
    }


	void MainScene::Update()
	{
        Scene::Update();

	}


    void MainScene::Render()
    {
        Scene::Render();

    }


	void MainScene::UnloadAssets()
	{

	}
}

