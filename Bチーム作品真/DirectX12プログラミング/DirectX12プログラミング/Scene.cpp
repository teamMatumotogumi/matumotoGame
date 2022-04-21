#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "Mathf.h"
#include "FrameResources.h"

// フレーム毎に更新される予定の定数たち
struct Scene::ConstantBufferLayoutForCamera
{
    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projMatrix;
};

Scene::Scene()
    : m_isUpdating(false)
{
    m_constantBufferForCamera = new ConstantBuffer(sizeof(ConstantBufferLayoutForCamera), nullptr, nullptr);
}

void Scene::LoadAssets()
{
    // 何もしない
}

void Scene::AddNewGameObject(GameObject* newGameObject)
{
    AddRootGameObject(newGameObject);

    if (m_isUpdating)
    {
        m_allTransforms.push_back(newGameObject->GetTransform());
    }
}

void Scene::AddRootGameObject(GameObject* rootGameObject)
{
    m_rootGameObjects.push_back(rootGameObject);
}

void Scene::RemoveRootGameObject(GameObject* rootGameObject)
{
    m_rootGameObjects.remove(rootGameObject);
}

void Scene::AddCamera(Camera* camera)
{
    m_allCameras.push_back(camera);
}

void Scene::RemoveCamera(Camera* camera)
{
    m_allCameras.remove(camera);
}


void Scene::Traverse(const std::function<void(Transform*)>& visitor)
{
    for (GameObject* rootGameObject : m_rootGameObjects)
    {
        rootGameObject->GetTransform()->Traverse(visitor);
    }
}


void Scene::Update()
{
    m_allTransforms.clear();

    static const std::function<void(Transform*)> visitor = [this](Transform* transform) { m_allTransforms.push_back(transform); };
    Traverse(visitor);

    m_isUpdating = true;
    for (size_t i = 0; i < m_allTransforms.size(); i++)
    {
        m_allTransforms[i]->GetGameObject()->Update();
    }
    m_isUpdating = false;
}


void Scene::Render()
{
    // フレームリソースセットの取得
    FrameResources* currentFrameResources = GraphicsEngine::Instance().GetCurrentFrameResources();

    // コマンドリストの取得
    ID3D12GraphicsCommandList* commandList = currentFrameResources->GetCommandList();

    for (Camera* camera : m_allCameras)
    {
        // 定数バッファへ定数データの書き込み
        // 
        // 行列データの場合は「転置」してから書き込もう。
        // (C/C++側は「行優先行列」、シェーダー側は「列優先行列」である為)
        // 
        ConstantBufferLayoutForCamera* mapped = (ConstantBufferLayoutForCamera*)m_constantBufferForCamera->Map();
        Mathf::Transpose(mapped->viewMatrix, camera->GetViewMatrix());
        Mathf::Transpose(mapped->projMatrix, camera->GetProjMatrix());
        m_constantBufferForCamera->Unmap();

        // 定数バッファをルートパラメーター0番にバインド
        commandList->SetGraphicsRootConstantBufferView(0, m_constantBufferForCamera->GetBufferViewDesc().BufferLocation);

        // カメラによるレンダリング
        camera->Render(currentFrameResources);
    }
}

void Scene::UnloadAssets()
{
    // 何もしない
}
