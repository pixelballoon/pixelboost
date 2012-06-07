#pragma once

#include "pixelboost/logic/component.h"

namespace pb
{
    
class Message;
    
class ParentTransformComponent : public Component
{
public:
    ParentTransformComponent(Entity* parent, pb::Uid destinationEntity);
    ~ParentTransformComponent();
    
private:
    void OnTransformChanged(Uid messageId, Message& message);
};
    
}
