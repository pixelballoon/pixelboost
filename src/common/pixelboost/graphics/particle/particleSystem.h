#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/logic/component.h"
#include "pixelboost/maths/hermiteCurve.h"

namespace pb
{
    
class Message;
class ParticleRenderable;
class ParticleSystem;
class ShaderPass;
class Sprite;
class Viewport;

struct Particle
{
    float StartLife;
	float Life;
	glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 RotationSpeed;
	glm::vec4 Color;
    float Scale;
    float Speed;
    float Variant;
    glm::vec3 Velocity;
};

class ParticleBehaviourDefinition
{
public:
	virtual void Update(ParticleSystem* system, float time) = 0;
};
    
class ParticleDefinitionEmitter
{
public:
    ParticleDefinitionEmitter();
    virtual ~ParticleDefinitionEmitter();
    
    virtual void Update(ParticleSystem* system, float time);
    
    virtual void CreateParticle(ParticleSystem* system, Particle* particle) = 0;
    
public:
    int EmitCount;
    float EmitSpeed;
    float Time;
};

class ParticleDefinitionEmitterCone : public ParticleDefinitionEmitter
{
public:
    ParticleDefinitionEmitterCone();
    
    virtual void CreateParticle(ParticleSystem* system, Particle* particle);
    
public:
    float Range;
    float Speed;
};
    
class ParticleDefinitionEmitterRectangle : public ParticleDefinitionEmitter
{
public:
    ParticleDefinitionEmitterRectangle();
    
    virtual void CreateParticle(ParticleSystem* system, Particle* particle);
    
public:
    glm::vec2 Size;
    bool RandomDirection;
};
    
class ParticleValueInit1D
{
public:
    void Set(float value);
    void Set(float min, float max);
    
    float GetRandValue();
    
    float MinValue;
    float MaxValue;
};
    
class ParticleValueInit3D
{
public:
    void Set(glm::vec3 value);
    void Set(glm::vec3 min, glm::vec3 max);
    
    glm::vec3 GetRandValue();
    
    glm::vec3 MinValue;
    glm::vec3 MaxValue;
};
    
class ParticleValueInitColor
{
public:
    void Set(glm::vec4 value);
    void Set(glm::vec4 min, glm::vec4 max);
    
    glm::vec4 GetRandValue();
    
    glm::vec4 MinValue;
    glm::vec4 MaxValue;
};

class ParticleValueModifier1D
{
public:
    virtual ~ParticleValueModifier1D();
    
    virtual float Evaluate(float x, float variant) = 0;
};
    
class ParticleValueModifier3D
{
public:
    virtual ~ParticleValueModifier3D();
    
    virtual glm::vec3 Evaluate(float x, float variant) = 0;
};
    
class ParticleValueModifierColor
{
public:
    virtual ~ParticleValueModifierColor();
    
    virtual glm::vec4 Evaluate(float x, float variant) = 0;
};
    
class ParticleValueCurve1D : public ParticleValueModifier1D
{
public:
    virtual float Evaluate(float x, float variant);
    
    HermiteCurve2D Curve;
};
    
class ParticleValueTwoCurve1D : public ParticleValueModifier1D
{
public:
    virtual float Evaluate(float x, float variant);
    
    HermiteCurve2D CurveMin;
    HermiteCurve2D CurveMax;
};
    
class ParticleValueCurve3D : public ParticleValueModifier3D
{
public:
    virtual glm::vec3 Evaluate(float x, float variant);
    
    HermiteCurve2D CurveX;
    HermiteCurve2D CurveY;
    HermiteCurve2D CurveZ;
};

class ParticleValueTwoCurve3D : public ParticleValueModifier3D
{
public:
    virtual glm::vec3 Evaluate(float x, float variant);
    
    HermiteCurve2D CurveMinX;
    HermiteCurve2D CurveMinY;
    HermiteCurve2D CurveMinZ;
    HermiteCurve2D CurveMaxX;
    HermiteCurve2D CurveMaxY;
    HermiteCurve2D CurveMaxZ;
};
    
class ParticleValueCurveColor : public ParticleValueModifierColor
{
public:
    virtual glm::vec4 Evaluate(float x, float variant);
    
    HermiteCurve2D CurveR;
    HermiteCurve2D CurveG;
    HermiteCurve2D CurveB;
    HermiteCurve2D CurveA;
};

class ParticleValueTwoCurveColor : public ParticleValueModifierColor
{
public:
    virtual glm::vec4 Evaluate(float x, float variant);
    
    HermiteCurve2D CurveMinR;
    HermiteCurve2D CurveMinG;
    HermiteCurve2D CurveMinB;
    HermiteCurve2D CurveMinA;
    HermiteCurve2D CurveMaxR;
    HermiteCurve2D CurveMaxG;
    HermiteCurve2D CurveMaxB;
    HermiteCurve2D CurveMaxA;
};
    
class ParticleMeshDefinition
{
public:
    
};
    
class ParticleSpriteDefinition
{
public:
    ParticleSpriteDefinition(const std::string& sprite);
    
    Sprite* SpriteDefinition;
};

class ParticleSystemDefinition
{
public:
    ParticleSystemDefinition();
    ~ParticleSystemDefinition();
    
	void Update(ParticleSystem* system, float time);
	void Render(ParticleSystem* system);
    
public:
    enum BlendMode
    {
        kBlendModeNormal,
        kBlendModeAdditive,
    };
    
    float Duration;
    
    ParticleValueInit1D StartLife;
    ParticleValueInit1D StartScale;
    ParticleValueInit1D StartSpeed;
    ParticleValueInit3D StartRotationSpeed;
    ParticleValueInitColor StartColor;

    glm::vec3 Gravity;
    int MaxParticles;
    
    BlendMode BlendMode;
    
    ParticleDefinitionEmitter* Emitter;
    
	ParticleSystemDefinition* SubSystemCreate;
	ParticleSystemDefinition* SubSystemDeath;

    ParticleValueModifierColor* ModifierColor;
    ParticleValueModifier1D* ModifierScale;
    ParticleValueModifier3D* ModifierVelocity;

	ParticleMeshDefinition* RenderMesh;
    ParticleSpriteDefinition* RenderSprite;
};
    
class ParticleSystem
{
public:
    ParticleSystem();
    ~ParticleSystem();
    
    void Update(float time);
    
    inline void UpdateParticle(Particle* particle, float time);
    
public:
    ParticleSystemDefinition* Definition;
    
    std::vector<Particle> Particles;
    std::vector<ParticleSystem*> SubSystem;
    
    ParticleSystem* Parent;
    glm::mat4x4 Transform;
};

class ParticleComponent : public Component
{
    PB_DECLARE_COMPONENT
    
public:
    ParticleComponent(Entity* entity);
    ~ParticleComponent();
    
    void SetRenderPass(RenderPass renderPass);
    void SetLayer(int layer);
    
    void SetLocalTransform(const glm::mat4x4& transform);
    
    ParticleSystem* GetSystem();
    
    void SetUseGlobalTime(bool useGlobalTime);
    
private:
    void OnTransformChanged(const Message& message);
    void OnUpdate(const Message& message);
    
    glm::mat4x4 _WorldMatrix;
    glm::mat4x4 _LocalTransform;
    ParticleRenderable* _Renderable;
    ParticleSystem* _System;
    
    bool _UseGlobalTime;
};

}
