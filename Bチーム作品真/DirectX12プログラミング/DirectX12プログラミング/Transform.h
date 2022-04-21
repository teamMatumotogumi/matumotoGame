#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <list>
#include <functional>

//---------------------------------------------------------------------------------------------------------------------------------------------
// トランスフォーム
//
//      ・3D空間での「スケール」「向き」「位置」を表すコンポーネントクラス。
//      ・一般的に「スケール」と「位置」は3次元ベクトルで表現され、「向き」は四元数で表現される。
//      ・「ワールド変換行列」を取得することができます。
//      ・「ワールド変換行列の逆行列」を取得することができます。
//      ・0または1個の親Transformを持ちます。
//      ・0個以上の子Transformを持ちます。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class Transform : public Component
{
private:
    std::list<Transform*> m_children;                   // 子Transformへの参照のリスト
    Transform* m_parent;                                // 親Transformへの参照
    DirectX::XMFLOAT3 m_localScale;                     // スケール(x,y,z)
    DirectX::XMFLOAT4 m_localRotation;                  // 向き(x,y,z,w)
    DirectX::XMFLOAT3 m_localPosition;                  // 位置(x,y,z)
    mutable DirectX::XMFLOAT4X4 m_localToWorldMatrix;   // [ローカル → ワールド]変換行列
    mutable DirectX::XMFLOAT4X4 m_worldToLocalMatrix;   // [ワールド → ローカル]変換行列
    friend class GameObject;                            // ゲームオブジェクトクラスは友達
    friend class Scene;                                 // シーンクラスは友達

private:
    // コンストラクタ
    Transform();

public:
    // このデータ型の情報を返します。
    static const TypeInfo& GetTypeInfo();

    // このインスタンスのデータ型の情報を返します。
    virtual const TypeInfo& GetTypeInfoOfInstance() const { return GetTypeInfo(); }

    // 親Transformを設定します。
    // (worldPositionStay == true)の場合、このTransformのワールド空間での位置が保たれます。
    // (worldPositionStay == false)の場合、親空間内での相対位置になる為、ワールド空間での位置が保たれません。
    void SetParent(Transform* parent, bool worldPositionStay = false);

    // 親Transformを取得します。
    Transform* GetParent() const { return m_parent; }

    // 子Transform数を取得します。
    uint32_t GetChildCount() const { return (uint32_t)m_children.size(); }

    // 子Transformリストへの参照を取得します。
    std::list<Transform*>& GetChildren() { return m_children; }

    // 子Transformリストへの参照を取得します。 (const版)
    const std::list<Transform*>& GetChildren() const { return m_children; }

    // 子Transformを切り離します。
    void DetachChild(Transform* child);

    // 全ての子Transformを切り離します。
    void DetachChildren();

    // スケールを設定します。
    void SetLocalScale(const DirectX::XMFLOAT3& localScale);

    // スケールを設定します。
    void SetLocalScale(float x, float y, float z);

    // スケールを取得します。
    const DirectX::XMFLOAT3& GetLocalScale() const { return m_localScale; }

    // 向きを設定します。
    void SetLocalRotation(const DirectX::XMFLOAT4& localRotation);

    // 向きを取得します。
    const DirectX::XMFLOAT4& GetLocalRotation() const { return m_localRotation; }

    // 位置を設定します。
    void SetLocalPosition(const DirectX::XMFLOAT3& localPosition);

    // 位置を設定します。
    void SetLocalPosition(float x, float y, float z);

    // 位置を取得します。
    const DirectX::XMFLOAT3& GetLocalPosition() const { return m_localPosition; }

    // 平行移動します。
    void Translate(const DirectX::XMFLOAT3& deltaPosition);

    // 平行移動します。
    void Translate(float dx, float dy, float dz);

    // [ローカル → ワールド]変換行列を取得します。
    const DirectX::XMFLOAT4X4& GetLocalToWorldMatrix() const;

    // [ワールド → ローカル]変換行列を取得します。
    const DirectX::XMFLOAT4X4& GetWorldToLocalMatrix() const;

    // 指定した名前を持つ子Transformを検索します。
    //      ・引数name に '/' の文字が含まれている場合は、パス名のように階層を走査します。
    //      ・子Transformが見つからない場合は nullptr を返します。
    Transform* Find(const char* name) const;

private:
    // 指定した名前のを持つ子Transformを検索します。(この関数は直接の子のみが対象です)
    Transform* GetChildByName(const std::string_view& name) const;

    // 各種行列を再計算します。
    void UpdateMatrices() const;

    // ローカル行列(スケーリング×回転×平行移動)を取得します。
    const DirectX::XMMATRIX GetLocalMatrix() const;

    // 
    void Traverse(const std::function<void(Transform*)>& visitor);
};
