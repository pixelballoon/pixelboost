#include "pixelboost/logic/message/transform.h"

using namespace pb;

PB_DEFINE_MESSAGE(pb::SetPositionMessage)

SetPositionMessage::SetPositionMessage(glm::vec3 position)
    : Message(0,0)
{
    _Position = position;
}

glm::vec3 SetPositionMessage::GetPosition()
{
    return _Position;
}

PB_DEFINE_MESSAGE(pb::SetRotationMessage)

SetRotationMessage::SetRotationMessage(glm::vec3 rotation)
    : Message(0,0)
{
    _Rotation = rotation;
}

glm::vec3 SetRotationMessage::GetRotation()
{
    return _Rotation;
}

PB_DEFINE_MESSAGE(pb::SetScaleMessage)

SetScaleMessage::SetScaleMessage(glm::vec3 scale)
    : Message(0,0)
{
    _Scale = scale;
}

glm::vec3 SetScaleMessage::GetScale()
{
    return _Scale;
}

PB_DEFINE_MESSAGE(pb::TransformChangedMessage)

TransformChangedMessage::TransformChangedMessage(Entity* entity, Component* component)
    : Message(entity, component)
{
    
}

TransformChangedMessage::~TransformChangedMessage()
{
    
}
