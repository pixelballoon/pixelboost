#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

namespace pb
{
    
class IndexBuffer;
class ParticleSystem;
class SpriteSheet;
class VertexBuffer;
    
class ParticleRenderable : public Renderable
{
public:
    ParticleRenderable(Uid entityId);
    ~ParticleRenderable();
    
    virtual Uid GetType();
    static Uid GetStaticType();
    
    virtual void CalculateBounds();
    virtual void CalculateWorldMatrix();
    
    virtual Shader* GetShader();
    
    void SetSystem(ParticleSystem* system);
    ParticleSystem* GetSystem();
    
    void SetTransform(const glm::mat4x4& transform);
    
private:
    ParticleSystem* _System;
    glm::mat4x4 _Transform;
    
    friend class ParticleRenderer;
};
    
struct EmitterConfig
{
    EmitterConfig();
    ~EmitterConfig();
    
    int emitCount;
    float particlesPerUpdate;
    
    float initialScale;
    float life;
    
    glm::vec2 startScale;
    glm::vec2 endScale;
    glm::vec4 startColor;
    glm::vec4 endColor;
    glm::vec3 minPosOffset;
    glm::vec3 maxPosOffset;
    glm::vec3 minRotOffset;
    glm::vec3 maxRotOffset;
    glm::vec3 minRotVelocity;
    glm::vec3 maxRotVelocity;
    glm::vec3 minPosVelocity;
    glm::vec3 maxPosVelocity;
    glm::vec3 gravity;
    
    typedef std::vector<std::string> SpriteList;
    SpriteList sprites;
    
private:
    std::shared_ptr<SpriteSheet> spriteSheet;
    
    friend class ParticleEmitter;
    friend class ParticleRenderer;
};

class ParticleRenderer : public IRenderer
{
public:
    ParticleRenderer();
    ~ParticleRenderer();
    
    static ParticleRenderer* Instance();
    
    virtual void Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix);
    
private:
    void RenderSystem(ParticleSystem* system, ShaderPass* shaderPass);
    
    IndexBuffer* _IndexBuffer;
    VertexBuffer* _VertexBuffer;
    
    int _MaxParticles;
    
    static ParticleRenderer* _Instance;
};

}
