#include "glm/gtc/random.hpp"

#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/particle/particleSystem.h"
#include "pixelboost/graphics/renderer/particle/particleRenderer.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/maths/matrixHelpers.h"

using namespace pb;

ParticleDefinitionEmitter::ParticleDefinitionEmitter()
{
    EmitCount = -1;
    EmitSpeed = 100.f;
    Time = 0;
}

ParticleDefinitionEmitter::~ParticleDefinitionEmitter()
{
    
}

void ParticleDefinitionEmitter::Update(ParticleSystem* system, float time)
{
    Time += time;
    
    if (EmitSpeed < 0.f)
        return;
    
    while (EmitCount != 0 && Time > 1.f/EmitSpeed)
    {
        if (system->Particles.size() == system->Definition->MaxParticles)
        {
            Time = 0.f;
            break;
        }
        
        if (EmitCount > 0)
            EmitCount--;
        
        Time -= 1.f/EmitSpeed;
        
        Particle particle;
        
        glm::vec4 position = system->Transform * glm::vec4(0,0,0,1);
        glm::vec4 rotation = system->Transform * glm::vec4(system->Definition->StartRotation.GetRandValue(), 0);
        
        particle.Position = glm::vec3(position.x, position.y, position.z);
        particle.Color = system->Definition->StartColor.GetRandValue();
        particle.StartLife = system->Definition->StartLife.GetRandValue();
        particle.Life = particle.StartLife;
        particle.Rotation = glm::vec3(rotation.x, rotation.y, rotation.z);
        particle.RotationSpeed = system->Definition->StartRotationSpeed.GetRandValue();
        particle.Scale = system->Definition->StartScale.GetRandValue();
        particle.Speed = system->Definition->StartSpeed.GetRandValue();
        particle.Variant = glm::linearRand(0.f, 1.f);
        
        CreateParticle(system, &particle);
        
        glm::vec4 direction = CreateRotateMatrix(kRotationOrder_XYZ, particle.Rotation) * glm::vec4(0,1,0,0);
        particle.Velocity = glm::vec3(direction.x, direction.y, direction.z) * particle.Speed;
        system->UpdateParticle(&particle, glm::linearRand(0.f, time));
        
        system->Particles.push_back(particle);
    }
}

ParticleDefinitionEmitterCone::ParticleDefinitionEmitterCone()
{
    Range = 45.f;
    Speed = 0;
}

void ParticleDefinitionEmitterCone::CreateParticle(ParticleSystem* system, Particle* particle)
{
    particle->Rotation += glm::vec3(glm::linearRand(-Range, Range), glm::linearRand(-Range, Range), glm::linearRand(-Range, Range));
}

ParticleDefinitionEmitterRectangle::ParticleDefinitionEmitterRectangle()
{
    
}

void ParticleDefinitionEmitterRectangle::CreateParticle(ParticleSystem* system, Particle* particle)
{
    glm::vec4 position = system->Transform * glm::vec4(glm::linearRand(-Size/2.f, Size/2.f), 0, 1);
    particle->Position = glm::vec3(position.x, position.y, position.z);
}

void ParticleValueInit1D::Set(float value)
{
    MinValue = value;
    MaxValue = value;
}

void ParticleValueInit1D::Set(float min, float max)
{
    MinValue = min;
    MaxValue = max;
}

float ParticleValueInit1D::GetRandValue()
{
    return glm::linearRand(MinValue, MaxValue);
}

void ParticleValueInit3D::Set(glm::vec3 value)
{
    MinValue = value;
    MaxValue = value;
}

void ParticleValueInit3D::Set(glm::vec3 min, glm::vec3 max)
{
    MinValue = min;
    MaxValue = max;
}

glm::vec3 ParticleValueInit3D::GetRandValue()
{
    return glm::linearRand(MinValue, MaxValue);
}

void ParticleValueInitColor::Set(glm::vec4 value)
{
    MinValue = value;
    MaxValue = value;
}

void ParticleValueInitColor::Set(glm::vec4 min, glm::vec4 max)
{
    MinValue = min;
    MaxValue = max;
}

glm::vec4 ParticleValueInitColor::GetRandValue()
{
    return glm::linearRand(MinValue, MaxValue);
}

ParticleValueModifier1D::~ParticleValueModifier1D()
{
    
}

float ParticleValueCurve1D::Evaluate(float x, float variant)
{
    return Curve.Evaluate(x);
}

float ParticleValueTwoCurve1D::Evaluate(float x, float variant)
{
    return glm::mix(CurveMin.Evaluate(x), CurveMax.Evaluate(x), variant);
}

ParticleValueModifier3D::~ParticleValueModifier3D()
{
    
}

glm::vec3 ParticleValueCurve3D::Evaluate(float x, float variant)
{
    return glm::vec3(CurveX.Evaluate(x), CurveY.Evaluate(x), CurveZ.Evaluate(x));
}

glm::vec3 ParticleValueTwoCurve3D::Evaluate(float x, float variant)
{
    return glm::mix(glm::vec3(CurveMinX.Evaluate(x), CurveMinY.Evaluate(x), CurveMinZ.Evaluate(x)), glm::vec3(CurveMaxX.Evaluate(x), CurveMaxY.Evaluate(x), CurveMaxZ.Evaluate(x)), variant);
}

ParticleSpriteDefinition::ParticleSpriteDefinition(const std::string& sprite)
{
    SpriteDefinition = Engine::Instance()->GetSpriteRenderer()->GetSprite(sprite);
}

ParticleSystemDefinition::ParticleSystemDefinition()
{
    Duration = 10.f;

    StartLife.Set(1.f);
    StartScale.Set(1.f);
    StartSpeed.Set(0.f);
    StartRotation.Set(glm::vec3(0,0,0));
    StartRotationSpeed.Set(glm::vec3(0,0,0));
    StartColor.Set(glm::vec4(1,1,1,1));
    
    Emitter = 0;
    
    ModifierScale = 0;
    ModifierVelocity = 0;
    
    Gravity = glm::vec3(0,0,0);
    MaxParticles = 100;
    
    BlendMode = kBlendModeAdditive;
}

ParticleSystemDefinition::~ParticleSystemDefinition()
{
    if (Emitter)
        delete Emitter;
    
    if (ModifierScale)
        delete ModifierScale;
    
    if (ModifierVelocity)
        delete ModifierVelocity;
}

ParticleSystem::ParticleSystem()
{
    Definition = new ParticleSystemDefinition();
    Particles.reserve(Definition->MaxParticles);
}

ParticleSystem::~ParticleSystem()
{
    delete Definition;
}

void ParticleSystem::Update(float time)
{
    for (std::vector<Particle>::iterator it = Particles.begin(); it != Particles.end();)
    {
        UpdateParticle(&(*it), time);
        
        if (it->Life < 0)
        {
            it = Particles.erase(it);
        } else {
            ++it;
        }
    }
    
    Definition->Emitter->Update(this, time);
    
    for (std::vector<ParticleSystem*>::iterator it = SubSystem.begin(); it != SubSystem.end(); ++it)
    {
        (*it)->Update(time);
    }
}

void ParticleSystem::UpdateParticle(Particle* particle, float time)
{
    particle->Life -= time;
    particle->Position += particle->Velocity * time;
    particle->Rotation += particle->RotationSpeed * time;
    particle->Velocity += Definition->Gravity * time;
    
    float life = 1.f-(particle->Life/particle->StartLife);
    
    if (Definition->ModifierVelocity != 0)
        particle->Velocity = Definition->ModifierVelocity->Evaluate(life, particle->Variant);
    
    if (Definition->ModifierScale != 0)
        particle->Scale = Definition->ModifierScale->Evaluate(life, particle->Variant);
}

PB_DEFINE_COMPONENT(ParticleComponent)

ParticleComponent::ParticleComponent(Entity* entity)
    : Component(entity)
{
    _System = new ParticleSystem();
    
    GetParent()->RegisterMessageHandler<TransformComponent>(MessageHandler(this, &ParticleComponent::OnTransformChanged));
    GetParent()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &ParticleComponent::OnUpdate));
    
    _Renderable = new ParticleRenderable(GetParentUid(), _System);
    GetScene()->GetSystemByType<RenderSystem>()->AddItem(_Renderable);
}

ParticleComponent::~ParticleComponent()
{
    GetParent()->UnregisterMessageHandler<TransformComponent>(MessageHandler(this, &ParticleComponent::OnTransformChanged));
    GetParent()->UnregisterMessageHandler<UpdateMessage>(MessageHandler(this, &ParticleComponent::OnUpdate));

    GetScene()->GetSystemByType<RenderSystem>()->RemoveItem(_Renderable);
    
    delete _Renderable;
}

void ParticleComponent::SetRenderPass(RenderPass renderPass)
{
    _Renderable->SetRenderPass(renderPass);
}

void ParticleComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

void ParticleComponent::SetLocalTransform(const glm::mat4x4& transform)
{
    _LocalTransform = transform;
}

ParticleSystem* ParticleComponent::GetSystem()
{
    return _System;
}

void ParticleComponent::OnTransformChanged(const Message& message)
{
    _WorldMatrix = GetParent()->GetComponentByType<TransformComponent>()->GetMatrix() * _LocalTransform;
}

void ParticleComponent::OnUpdate(const Message& message)
{
    const pb::UpdateMessage& updateMessage = static_cast<const UpdateMessage&>(message);
    
    _System->Transform = GetParent()->GetComponentByType<TransformComponent>()->GetMatrix() * _LocalTransform;
    _System->Update(updateMessage.GetGameDelta());
}
