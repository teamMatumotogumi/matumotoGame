#pragma once
#include "Object.h"

// 前方宣言
class GameObject;
class Transform;
class MonoBehaviour;

//---------------------------------------------------------------------------------------------------------------------------------------------
// コンポーネントクラス
//
//   ・ゲームオブジェクトに様々な機能を与える部品(コンポーネント)を表す。
//   ・他のクラスから継承されることを前提とした基底クラス。
//   ・所有者であるゲームオブジェクトへの参照を持ちます。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class Component : public Object
{
private:
    GameObject* m_owner;        // このコンポーネントを所有するゲームオブジェクトへの参照
    friend class GameObject;    // ゲームオブジェクトクラスは友達

protected:
    // コンストラクタ
    Component();

    // 仮想デストラクタ
    virtual ~Component();

    // ゲームオブジェクトにアタッチされた際に呼び出されます。
    virtual void OnAttach();

    // ゲームオブジェクトからデタッチされた際に呼び出されます。
    virtual void OnDetach();

    // 更新処理を行う関数です。
    //   ・この関数は1秒間に約60回の頻度で所有者であるゲームオブジェクトから呼び出されます。
    //   ・この関数はダミーなので何もしません。
    //     必要であれば継承先のクラスでオーバーライドしてください。
    virtual void Update();

    // 描画処理を行う関数です。
    //   ・この関数は1秒間に約60回の頻度で所有者であるゲームオブジェクトから呼び出されます。
    //   ・この関数はダミーなので何もしません。
    //     必要であれば継承先のクラスでオーバーライドしてください。
    virtual void Render();

private:
    // このコンポーネントを所有するゲームオブジェクトを設定します。
    void SetGameObject(GameObject* owner) { m_owner = owner; }

public:
    // このデータ型の情報を返します。
    static const TypeInfo& GetTypeInfo();

    // このインスタンスのデータ型の情報を返します。
    virtual const TypeInfo& GetTypeInfoOfInstance() const { return GetTypeInfo(); }

    // このコンポーネントを所有するゲームオブジェクトを取得します。
    GameObject* GetGameObject() const { return m_owner; }

    // Transformコンポーネントを取得します。 (ショートカット)
    Transform* GetTransform() const;

    // このオブジェクトの識別名を設定します。
    void SetName(const std::string& name) override;

    // このオブジェクトの識別名を取得します。
    const std::string& GetName() const override;

    // このコンポーネントがMonoBehaviourを継承している場合は true を返します。
    virtual MonoBehaviour* AsMonoBehaviour() const { return nullptr; }

    // 同一コンポーネントの重複を禁止する場合は true を返します。
    static bool DisallowMultipleComponent() { return false; }
};

