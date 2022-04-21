#include "Component.h"
#include "GameObject.h"

const TypeInfo& Component::GetTypeInfo()
{
    static TypeInfo typeInfo(TypeID::Component, "Component");
    return typeInfo;
}

Component::Component()
    : m_owner(nullptr)
{
}

Component::~Component()
{
}

void Component::OnAttach()
{
    // 何もしない
}

void Component::Update()
{
    // 何もしない
}

void Component::Render()
{
    // 何もしない
}

void Component::OnDetach()
{
    // 何もしない
}

Transform* Component::GetTransform() const
{
    return m_owner->GetTransform();
}

void Component::SetName(const std::string& name)
{
    m_owner->SetName(name);
}

const std::string& Component::GetName() const
{
    return m_owner->GetName();
}

