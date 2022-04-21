#pragma once
#include <string>
#include <wrl.h>
#include "ReferenceCounter.h"
#include "TypeInfo.h"

// 前方宣言
class Transform;
class Vector3;
class Quaternion;


//---------------------------------------------------------------------------------------------------------------------------------------------
// オブジェクトクラス
// 
//      ・ほとんどのリソース系オブジェクトの基底クラス。
//      ・オブジェクト識別名やインスタンスIDを管理する。
//---------------------------------------------------------------------------------------------------------------------------------------------
class Object : public ReferenceCounter
{
private:
    static uint64_t     s_serialInstanceID;     // オブジェクト生成時に割り当てられるID
    uint64_t            m_instanceID;           // 生成時ID

protected:
    // コンストラクタ
    Object();

    // 仮想デストラクタ
    virtual ~Object();

public:
    // このデータ型の情報を返します。
    static const TypeInfo& GetTypeInfo();

    // このインスタンスのデータ型の情報を返します。
    virtual const TypeInfo& GetTypeInfoOfInstance() const { return GetTypeInfo(); }

    // このオブジェクトの識別名を設定します。
    virtual void SetName(const std::string& name) = 0;

    // このオブジェクトの識別名を取得します。
    virtual const std::string & GetName() const = 0;

    // オブジェクトのインスタンスIDを返します。
    // インスタンスIDは常にユニークな値であることが保証されます。
    uint64_t GetInstanceID() const;

    // ゲームオブジェクト、コンポーネントやアセットを破棄します。
    static void Destroy(Object& object, float time = 0.0f);

    // ゲームオブジェクト、コンポーネントやアセットを破棄します。
    static void DestroyImmediate(Object& object, bool allowDestroyingAssets = false);

    // 新しいシーンのロード時に target オブジェクトを破棄しません。
    static void DontDestroyOnLoad(Object& target);

    // originalを基にT型のインスタンスを生成します。
    template<typename T>
    static T* Instantiate(const T& original, const Transform* parent = nullptr, bool instantiateInWorldSpace = false);

    // originalを基にT型のインスタンスを生成します。
    template<typename T>
    static T* Instantiate(const T& original, const Vector3& position, const Quaternion& rotation, const Transform* parent = nullptr);

protected:
    // 
    virtual Object* Instantiate(const Object& original, const Vector3* position, const Quaternion* rotation, const Transform* parent, bool instantiateInWorldSpace) { return nullptr; }
    virtual void Destroy(float time) {}
    virtual void DestroyImmediate(bool allowDestroyingAssets) {}
    virtual void DontDestroyOnLoad() {}
};


template<typename T>
inline T* Object::Instantiate(const T& original, const Transform* parent, bool instantiateInWorldSpace)
{
    return (T*)Instantiate(original, nullptr, nullptr, parent, instantiateInWorldSpace);
}


template<typename T>
inline T* Object::Instantiate(const T& original, const Vector3& position, const Quaternion& rotation, const Transform* parent)
{
    return (T*)Instantiate(original, &position, &rotation, parent, false);
}
