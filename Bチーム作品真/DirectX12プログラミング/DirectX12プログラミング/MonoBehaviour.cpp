#include "Precompiled.h"
#include "MonoBehaviour.h"

const TypeInfo& MonoBehaviour::GetTypeInfo()
{
    static TypeInfo typeInfo(TypeID::MonoBehaviour, "MonoBehaviour");
    return typeInfo;
}
