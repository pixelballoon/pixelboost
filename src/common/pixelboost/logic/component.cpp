#include "pixelboost/logic/component.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

Component::Component(Entity* parent)
    : _Parent(parent)
{
    _Uid = parent->GenerateComponentId();
    parent->AddComponent(this);
}

Component::~Component()
{

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
