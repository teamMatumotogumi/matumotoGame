#pragma once
#include <vector>
#include <cassert>
#include "Object.h"
#include "Vector2.h"
#include "Vector3.h"

// 前方宣言
class TypeInfo;
class Component;
class Transform;
class Texture2D;
class MonoBehaviour;
class Rect;
class Scene;

//---------------------------------------------------------------------------------------------------------------------------------------------
// ゲームオブジェクトクラス
//
//   ・コンポーネント(部品)を追加していくことで様々な機能を持たせることができる。
//   ・このクラスは継承されることを想定していない。 (C++ではfinalキーワード)
//   ・必須コンポーネントであるTransformコンポーネントだけは最初から追加済み。
//   ・Transformコンポーネントをユーザーが追加することはできません。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class GameObject final : public Object
{
private:
    bool m_isActiveSelf;                    // このゲームオブジェクトがアクティブな場合は true
    Scene* m_scene;                         // このゲームオブジェクトが所属するシーンへの参照
    std::string m_name;                     // このゲームオブジェクトの名前
    std::vector<Component*> m_components;   // コンポーネント配列
    Transform* m_transform;                 // Transformコンポーネントへのショートカット
    friend class Scene;                     // Sceneクラスは友達
    friend class Camera;                    // Cameraクラスは友達

public:
    // このデータ型の情報を返します。
    static const TypeInfo& GetTypeInfo();

    // このインスタンスのデータ型の情報を返します。
    virtual const TypeInfo& GetTypeInfoOfInstance() const { return GetTypeInfo(); }

    // スプライト描画機能を持ったゲームオブジェクトを作成します。
    static GameObject* CreateWithSprite(const char* gameObjectName, Texture2D* texture, const Rect& rect, const Vector2& pivot = Vector2(0.0f, 0.0f), float pixelsPerUnit = 100.0f, const Vector3& localPosition = Vector3(0, 0, 0), Transform* parent = nullptr);

    // デフォルトコンストラクタ
    GameObject();

    // 引数付きコンストラクタ
    GameObject(const char* name);

    // このオブジェクトの識別名を設定します。
    void SetName(const std::string& name) override { m_name = name; }

    // このオブジェクトの識別名を取得します。
    const std::string& GetName() const override { return m_name; }

    // このゲームオブジェクト自身がアクティブな場合は true を返します。
    bool IsActiveSelf() const { return m_isActiveSelf; }

    // このゲームオブジェクトがシーン内でアクティブな場合は true を返します。
    // (これは、このゲームオブジェクト自身と全ての親がアクティブな場合です)
    bool IsActiveInHierarchy() const;

    // このゲームオブジェクトが所属するシーンを取得します。
    Scene* GetScene() const { return m_scene; }

    // このゲームオブジェクトの持つTransformコンポーネントを取得します。 (ショートカット関数)
    Transform* GetTransform() const { return m_transform; }

    // このゲームオブジェクトにコンポーネントを追加します。
    template<typename ComponentType>
    ComponentType* AddComponent();

    // 指定したタイプと一致するコンポーネントを取得します。
    template<typename ComponentType>
    ComponentType* GetComponent() const;

    // 指定した名前を持つゲームオブジェクトを検索します。
    //      ・複数のシーンが実行されている場合はそれら全てのシーン内を検索します。
    //      ・この関数はアクティブなゲームオブジェクトのみを返します。
    //      ・引数name に '/' の文字が含まれている場合は、パス名のように階層を走査します。
    //      ・ゲームオブジェクトが見つからない場合は nullptr を返します。
    //      ・子ゲームオブジェクトを検索する場合は Transform::Find()を使用した方が簡単です。
    static GameObject* Find(const char* name);

private:
    // デストラクタ
    ~GameObject();

    // このゲームオブジェクトにコンポーネントを追加します。 (内部専用バージョン)
    template<typename ComponentType>
    ComponentType* InternalAddComponent();

    // 明示的特殊化によりTransformコンポーネントの追加を抑制する。
    // (クラス外部からTransformコンポーネントを追加されると困る)
    template<>
    Transform* AddComponent() { assert(0); return nullptr; }

    // このゲームオブジェクトが所有する全てのコンポーネントに対して更新命令を出します。
    void Update();

    // このゲームオブジェクトが所有する全てのコンポーネントに対して描画命令を出します。
    void Render();

    // このゲームオブジェクトが所有する全ての MonoBehaviour に対してメッセージを送信します。
    void BroadcastMessage(const std::function<void(MonoBehaviour*)>& clojure);
};









#include "MonoBehaviour.h"

//--------------------------------------------------------------------------------------
// ※注意
// 
//  「クラステンプレート」や「関数テンプレート」の実装はソースファイル(.cpp)に記述してはいけない。
//   それらの利用場所から見える場所に記述しよう。
// 
//--------------------------------------------------------------------------------------

template<typename ComponentType>
inline ComponentType* GameObject::AddComponent()
{
    return InternalAddComponent<ComponentType>();
}


template<typename ComponentType>
inline ComponentType* GameObject::InternalAddComponent()
{
    // ComponentTypeは未知のデータ型であり、この関数の呼び出し時にデータ型が決定される。
    // ComponentTypeはComponentクラスを継承しているはず・・・
    ComponentType* component = new ComponentType();

    // コンポーネントの所有者として自分を設定する。
    component->SetGameObject(this);
    component->OnAttach();

    // ComponentTypeが何型かは不明だが可変長配列に格納しておく。
    m_components.push_back(component);

    // MonoBehaviour派生コンポーネントの場合はこのタイミングで Awake()、Start() しておく。
    // (Unityとはシーンの仕様が異なる為)
    if (MonoBehaviour* monoBehaviour = component->AsMonoBehaviour())
    {
        monoBehaviour->Awake();
        monoBehaviour->Start();
    }

    return component;
}


template<typename ComponentType>
inline ComponentType* GameObject::GetComponent() const
{
    // 検索したいコンポーネントのデータ型IDを取得する
    const TypeID searchTargetTypeID = ComponentType::GetTypeInfo().GetTypeID();

    // コンポーネント配列内から同じデータ型IDを持つコンポーネントを検索する
    // (コンポーネント数が多い場合、この検索方法はあまり効率的ではないかもしれない)
    for (Component* component : m_components)
    {
        // 現在列挙中のコンポーネントのデータ型IDを取得する
        const TypeID currentTypeID = component->GetTypeInfoOfInstance().GetTypeID();

        // データ型IDが一致するか？
        if (currentTypeID == searchTargetTypeID)
        {
            // コンポーネントが見つかった！
            return (ComponentType*)component;
        }
    }

    // 見つからなかった…
    return nullptr;
}


