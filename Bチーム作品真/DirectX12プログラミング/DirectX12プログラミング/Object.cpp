#include "Object.h"
#include <cassert>

// 静的メンバ変数の実体を宣言
uint64_t Object::s_serialInstanceID = 0;


const TypeInfo& Object::GetTypeInfo()
{
    static TypeInfo typeInfo(TypeID::Object, "Object");
    return typeInfo;
}


Object::Object()
    : m_instanceID(s_serialInstanceID++)
{
}


Object::~Object()
{
}


uint64_t Object::GetInstanceID() const
{
    return m_instanceID;
}


void Object::Destroy(Object& object, float t)
{
    assert(!"Object::Destroy() is not yet implemented.");
    object.Destroy(t);
}


void Object::DestroyImmediate(Object& object, bool allowDestroyingAssets)
{
    assert(!"Object::DestroyImmediate() is not yet implemented.");
    object.DestroyImmediate(allowDestroyingAssets);
}


void Object::DontDestroyOnLoad(Object& target)
{
    assert(!"Object::DontDestroyOnLoad() is not yet implemented.");
    target.DontDestroyOnLoad();
}
