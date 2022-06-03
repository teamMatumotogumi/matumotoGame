#include "Precompiled.h"
#include "GameScene.h"

void MatumotoGame::GameScene::LoadAssets()
{
	// シーンルートのゲームオブジェクトを作成
	m_sceneRoot = new GameObject("シーンルート");

    // メインカメラを作成
    CreateMainCamera(m_sceneRoot->GetTransform());

    // 背景を作成
    CreateBackground(m_sceneRoot->GetTransform());

   

}

void MatumotoGame::GameScene::CreateMainCamera(Transform* parent)
{
    // メインカメラ用のゲームオブジェクトを作成
    GameObject* mainCamera = new GameObject("メインカメラ");

    // カメラコンポーネントを追加する
    m_mainCameraComponent = mainCamera->AddComponent<Camera>();
    m_mainCameraComponent->SetOrthographic(true);
    m_mainCameraComponent->SetOrthographicSize(1080 / 2);
    m_mainCameraComponent->SetAspect(1920 / 1080.0f);
    m_mainCameraComponent->SetNearClipPlane(0.3f);
    m_mainCameraComponent->SetFarClipPlane(1000.0f);
    m_mainCameraComponent->SetClearFlags(CameraClearFlags::SolidColor);
    m_mainCameraComponent->SetBackgroundColor(Color::CornflowerBlue);
    m_mainCameraComponent->SetNormalizedViewportRect(Rect(0.0f, 0.0f, 1.0f, 1.0f));

    // 位置の変更
    Transform* transform = mainCamera->GetTransform();
    transform->SetParent(parent);
    transform->SetLocalPosition(1920/2, 1080 / 2, -10);
}

void MatumotoGame::GameScene::CreateBackground(Transform* parent)
{
    // 背景1枚目
    Texture2D* background01Texture = Texture2D::FromFile(L"Assets/Stage(B)/背景編集済み1.png");
    m_object1 = GameObject::CreateWithSprite("背景1枚目", background01Texture, Rect(0, 0, 1920, 1080), Vector2(0.0f, 0.0f), 1.0f, Vector3(0, 0, 100), parent);
    // 背景2枚目
    Texture2D* background02Texture = Texture2D::FromFile(L"Assets/Stage(B)/back2.png");
  m_object2 =  GameObject::CreateWithSprite("背景2枚目", background02Texture, Rect(0, 0, 1920, 1080), Vector2(0.0f, 0.0f), 1.0f, Vector3(1920, 0, 100), parent);

}



void MatumotoGame::GameScene::Update()
{
    // 位置の変更
    Transform* transform = m_mainCameraComponent->GetTransform();
    DirectX::XMFLOAT3 cameraPos = transform->GetLocalPosition();
    cameraPos.x += 6;
    
    transform->SetLocalPosition(cameraPos);

    // 今カメラがどこを移しているかを求め、画像1を移動させる
    float a = (int)((cameraPos.x + 1920 -(1920 / 2)) / 3840) * 3840;
    m_object1->GetTransform()->SetLocalPosition( a,  0,  0);

    // カメラを求め、2枚目を移動させる
    float b = (int)((cameraPos.x + 0 - (1920 / 2)) / 3840) * 3840 + 1920;
    m_object2->GetTransform()->SetLocalPosition(b, 0, 0);
   
    Scene::Update();
}

void MatumotoGame::GameScene::Render()
{
    Scene::Render();
}

void MatumotoGame::GameScene::UnloadAssets()
{

}


