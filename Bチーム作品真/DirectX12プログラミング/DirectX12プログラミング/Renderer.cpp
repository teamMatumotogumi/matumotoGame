#include "Renderer.h"


Renderer::Renderer()
    : m_isEnabled(true)
    , m_isVisible(true)
    , m_sortingLayerID(0)
    , m_sortingOrder(0)
{

}


const TypeInfo& Renderer::GetTypeInfo()
{
    static TypeInfo typeInfo(TypeID::Renderer, "Renderer");
    return typeInfo;
}




