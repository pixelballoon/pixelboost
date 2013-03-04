#include "pixelboost/logic/component.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/scripting/lua.h"

using namespace pb;

Component::Component(Entity* parent)
    : _Parent(parent)
    , _State(kComponentCreated)
{
    _Uid = parent->GenerateComponentId();
    parent->AddComponent(this);
}

Component::~Component()
{

}

void Component::RegisterLuaClass(lua_State* state)
{
    luabridge::getGlobalNamespace(state)
    .beginNamespace("pb")
        .beginClass<Component>("Component")
            .addFunction("GetParent", &Component::GetParent)
            .addFunction("GetParentUid", &Component::GetParentUid)
        .endClass()
    .endNamespace();
}

bool Component::IsA(Uid type) const
{
    return type == GetType();
}

Uid Component::GetType() const
{
    return GetStaticType();
}

Uid Component::GetStaticType()
{
    return TypeHash("pb::Component");
}

Uid Component::GetUid()
{
    return _Uid;
}

Scene* Component::GetScene()
{
    return _Parent->GetScene();
}

Entity* Component::GetParent()
{
    return _Parent;
}

pb::Uid Component::GetParentUid()
{
    return _Parent->GetUid();
}
