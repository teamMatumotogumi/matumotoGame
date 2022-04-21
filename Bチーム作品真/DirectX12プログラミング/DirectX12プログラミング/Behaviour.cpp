#include "Precompiled.h"
#include "Behaviour.h"


Behaviour::Behaviour()
    : m_enabled(true)
{
}


const TypeInfo& Behaviour::GetTypeInfo()
{
    static TypeInfo typeInfo(TypeID::Behaviour, "Behaviour");
    return typeInfo;
}


bool Behaviour::IsActiveAndEnabled() const
{
    return (m_enabled && GetGameObject()->IsActiveSelf());
}
