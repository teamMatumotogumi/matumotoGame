#include "GameObject.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Scene.h"

const TypeInfo& GameObject::GetTypeInfo()
{
    static TypeInfo typeInfo(TypeID::GameObject, "GameObject");
    return typeInfo;
}


GameObject* GameObject::CreateWithSprite(const char* gameObjectName, Texture2D* texture, const Rect& rect, const Vector2& pivot, float pixelsPerUnit, const Vector3& localPosition, Transform* parent)
{
    // ゲームオブジェクトを作成
    GameObject* gameObject = new GameObject(gameObjectName);

    // スプライトを作成
    Sprite* sprite = Sprite::Create(texture, rect, pivot, pixelsPerUnit);

    // スプライトレンダラーの追加
    SpriteRenderer* spriteRenderer = gameObject->AddComponent<SpriteRenderer>();
    spriteRenderer->SetSprite(sprite);

    // 位置の変更
    Transform* transform = gameObject->GetTransform();
    if (parent)
    {
        transform->SetParent(parent, false);
    }
    transform->SetLocalPosition(localPosition.x, localPosition.y, localPosition.z);
    return gameObject;
}


GameObject::GameObject()
    : GameObject(nullptr)
{
}


GameObject::GameObject(const char* name)
    : m_isActiveSelf(true)
    , m_scene(nullptr)
    , m_transform(nullptr)
{
    if (name)
    {
        SetName(name);
    }

    // 所属するシーン
    m_scene = SceneManager::GetActiveScene();

    // Transform型オブジェクトだけは最初から追加しておく。
    m_transform = InternalAddComponent<Transform>();

    m_scene->AddNewGameObject(this);
}


bool GameObject::IsActiveInHierarchy() const
{
    // 自身と全ての親を走査する
    Transform* current = GetTransform();
    while (current)
    {
        if (!current->GetGameObject()->IsActiveSelf())
            return false;

        current = current->GetParent();
    }
    return true;
}


GameObject* GameObject::Find(const char* name)
{
    return nullptr;
}


GameObject::~GameObject()
{
    // 所有する全てのコンポーネントを解放
    for (auto& component : m_components)
    {
        component->Release();
    }
    m_components.clear();
}


void GameObject::Update()
{
    // 全てのコンポーネントを走査する
    for (Component* component : m_components)
    {
        component->Update();
    }

    // 子Transformに伝播させる
    std::list<Transform*> children = m_transform->GetChildren();
    for (Transform* child : children)
    {
        child->GetGameObject()->Update();
    }
}


void GameObject::Render()
{
    // 全てのコンポーネントを走査する
    for (Component* component : m_components)
    {
        component->Render();
    }

    // 子Transformに伝播させる
    const std::list<Transform*>& children = m_transform->GetChildren();
    for (Transform* child : children)
    {
        child->GetGameObject()->Render();
    }
}


void GameObject::BroadcastMessage(const std::function<void(MonoBehaviour* monoBehaviour)>& clojure)
{
    // 全てのコンポーネントを走査する
    for (Component* component : m_components)
    {
        // MonoBehaviourを継承したスクリプトの場合は
        if (MonoBehaviour* monoBehaviour = component->AsMonoBehaviour())
        {
            clojure(monoBehaviour);
        }
    }

    // 子Transformに伝播させる
    const std::list<Transform*>& children = m_transform->GetChildren();
    for (Transform* child : children)
    {
        child->GetGameObject()->BroadcastMessage(clojure);
    }
}
