#pragma once

#include <string>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/logic/component.h"

namespace pb
{
    
class Message;
class ParticleEmitter;
class ParticleRenderable;

class ParticleEmitterComponent : public Component
{
public:
    ParticleEmitterComponent(Entity* parent, int maxParticles=50);
    virtual ~ParticleEmitterComponent();
    
    Uid GetType();
    static Uid GetStaticType();
    
    glm::vec2 GetSize();
    
    void SetLayer(int layer);

    ParticleEmitter* GetEmitter();
    
private:
    void OnUpdate(Uid sender, const Message& message);
    void OnTransformChanged(Uid sender, const Message& message);
    void UpdateTransform();
    
    ParticleRenderable* _Renderable;
};
    
}
