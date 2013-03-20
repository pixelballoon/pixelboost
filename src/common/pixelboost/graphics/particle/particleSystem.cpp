#include "glm/gtc/random.hpp"
#include "glm/gtc/swizzle.hpp"

#include "pixelboost/graphics/particle/particleSystem.h"
#include "pixelboost/graphics/renderer/particle/particleRenderer.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/resources/spriteSheetResource.h"
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
        
        particle.Color = system->Definition->StartColor.GetRandValue();
        particle.StartLife = system->Definition->StartLife.GetRandValue();
        particle.Life = particle.StartLife;
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
    particle->Position = glm::swizzle<glm::X, glm::Y, glm::Z>(system->Transform * glm::vec4(0,0,0,1));
    particle->Rotation = glm::swizzle<glm::X, glm::Y, glm::Z>(system->Transform * glm::vec4(glm::linearRand(-Range, Range), glm::linearRand(-Range, Range), glm::linearRand(-Range, Range), 0));
}

ParticleDefinitionEmitterRectangle::ParticleDefinitionEmitterRectangle()
{
    RandomDirection = true;
}

void ParticleDefinitionEmitterRectangle::CreateParticle(ParticleSystem* system, Particle* particle)
{
    particle->Position = glm::swizzle<glm::X, glm::Y, glm::Z>(system->Transform * glm::vec4(glm::linearRand(-Size/2.f, Size/2.f), 0, 1));
    
    if (RandomDirection)
    {
        particle->Rotation = glm::vec3(0, 0, glm::linearRand(-180.f, 180.f));
    } else
    {
        particle->Rotation = glm::swizzle<glm::X, glm::Y, glm::Z>(system->Transform * glm::vec4(0,0,0,1));
    }
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

ParticleValueModifierColor::~ParticleValueModifierColor()
{
    
}

glm::vec4 ParticleValueCurveColor::Evaluate(float x, float variant)
{
    return glm::vec4(CurveR.Evaluate(x), CurveG.Evaluate(x), CurveB.Evaluate(x), CurveA.Evaluate(x));
}

glm::vec4 ParticleValueTwoCurveColor::Evaluate(float x, float variant)
{
    return glm::mix(glm::vec4(CurveMinR.Evaluate(x), CurveMinG.Evaluate(x), CurveMinB.Evaluate(x), CurveMinA.Evaluate(x)), glm::vec4(CurveMaxR.Evaluate(x), CurveMaxG.Evaluate(x), CurveMaxB.Evaluate(x), CurveMaxA.Evaluate(x)), variant);
}

ParticleSpriteDefinition::ParticleSpriteDefinition(const std::string& name)
{
    SpriteName = name;
    SpriteDefinition = 0;
}

ParticleSystemDefinition::ParticleSystemDefinition()
{
    Duration = 10.f;

    StartLife.Set(1.f);
    StartScale.Set(1.f);
    StartSpeed.Set(0.f);
    StartRotationSpeed.Set(glm::vec3(0,0,0));
    StartColor.Set(glm::vec4(1,1,1,1));
    
    Emitter = 0;
    
    ModifierColor = 0;
    ModifierScale = 0;
    ModifierVelocity = 0;
    
    Gravity = glm::vec3(0,0,0);
    MaxParticles = 100;
    
    BlendMode = kBlendModeAdditive;
    
    RenderMesh = 0;
    RenderSprite = 0;
}

ParticleSystemDefinition::~ParticleSystemDefinition()
{
    if (Emitter)
        delete Emitter;
    
    if (ModifierColor)
        delete ModifierColor;
    
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
    
    if (Definition->ModifierColor != 0)
        particle->Color = Definition->ModifierColor->Evaluate(life, particle->Variant);
    
    if (Definition->ModifierVelocity != 0)
        particle->Velocity = Definition->ModifierVelocity->Evaluate(life, particle->Variant);
    
    if (Definition->ModifierScale != 0)
        particle->Scale = Definition->ModifierScale->Evaluate(life, particle->Variant);
}

PB_DEFINE_COMPONENT(pb::ParticleComponent)

ParticleComponent::ParticleComponent(Entity* parent)
    : RenderableComponent(parent)
    , _UseGlobalTime(false)
{
    _System = new ParticleSystem();
    
    GetRenderable()->SetSystem(_System);
    
    GetEntity()->RegisterMessageHandler<UpdateMessage>(MessageHandler(this, &ParticleComponent::OnUpdate));
}

ParticleComponent::~ParticleComponent()
{
    if (_SpriteSheet)
    {
        _SpriteSheet->SignalResourceLoaded.Disconnect(this, &ParticleComponent::OnResourceLoaded);
        _SpriteSheet->SignalResourceUnloading.Disconnect(this, &ParticleComponent::OnResourceUnloading);
    }
    
    GetEntity()->UnregisterMessageHandler<UpdateMessage>(MessageHandler(this, &ParticleComponent::OnUpdate));
}

ParticleSystem* ParticleComponent::GetSystem()
{
    return _System;
}

void ParticleComponent::SetSpriteSheet(const std::string& filename, const std::string& pool)
{
    _SpriteSheet = ResourceManager::Instance()->GetPool(pool)->GetResource<SpriteSheetResource>(filename);
    _SpriteSheet->SignalResourceLoaded.Connect(this, &ParticleComponent::OnResourceLoaded);
    _SpriteSheet->SignalResourceUnloading.Connect(this, &ParticleComponent::OnResourceUnloading);
    
    if (_SpriteSheet->GetState() == kResourceStateReady)
    {
        OnResourceLoaded(_SpriteSheet.get(), false);
    }
}

void ParticleComponent::SetUseGlobalTime(bool useGlobalTime)
{
    _UseGlobalTime = useGlobalTime;
}

void ParticleComponent::OnUpdate(const Message& message)
{
    auto updateMessage = message.As<UpdateMessage>();
    
    _System->Update(_UseGlobalTime ? updateMessage.GetTimeDelta() : updateMessage.GetGameDelta());
}

void ParticleComponent::OnResourceLoaded(Resource* resource, bool error)
{
    if (!error && resource == _SpriteSheet.get())
    {
        if (_System->Definition->RenderSprite)
        {
            _System->Definition->RenderSprite->SpriteDefinition = _SpriteSheet->GetSpriteSheet()->GetSprite(_System->Definition->RenderSprite->SpriteName);
        }
    }
}

void ParticleComponent::OnResourceUnloading(Resource* resource)
{
    if (resource == _SpriteSheet.get())
    {
        if (_System->Definition->RenderSprite)
        {
            _System->Definition->RenderSprite->SpriteDefinition = 0;
        }
    }
}
