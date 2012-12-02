#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/misc/pointer.h"

namespace pb
{
    
class IndexBuffer;
class ParticleEmitter;
class ParticleModifier;
class SpriteSheet;
class VertexBuffer;
    
class ParticleRenderable : public Renderable
{
public:
    ParticleRenderable(Uid entityId, int maxParticles);
    ~ParticleRenderable();
    
    virtual Uid GetType();
    static Uid GetStaticType();
    
    virtual void CalculateBounds();
    virtual void CalculateWorldMatrix();
    
    virtual Shader* GetShader();
    
    ParticleEmitter* GetEmitter();
    
private:
    ParticleEmitter* _Emitter;
    
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
    
class ParticleEmitter
{
public:
    ParticleEmitter(int maxParticles=50);
    ~ParticleEmitter();
    
    void Update(float time);
    void Render(Viewport* viewport, ShaderPass* shaderPass);
    
public:
    struct Particle
    {
        Particle(EmitterConfig* config);
        Particle(const Particle& particle);
        ~Particle();
        
        Particle& operator=(const Particle& rhs);
        
        std::string sprite;
        
        EmitterConfig* emitterConfig;
        
        float life;
        float totalLife;
        
        glm::vec3 rotation;
        glm::vec3 position;
        
        glm::vec3 rotationVelocity;
        glm::vec3 positionVelocity;
        
    private:
        void Assign(const Particle& rhs);
    };

public:
    typedef std::vector<ParticleModifier*> ModifierList;
    typedef std::vector<Particle> ParticleList;
    
    void AddModifier(ParticleModifier* modifier);
    
    bool IsFinished();
    int GetNumParticles();
    void ResetSpawnCount();
    
    void LoadSpriteSheet(FileLocation location, const std::string& file, const std::string& extension, bool createMips);
    void SetSpriteSheet(std::shared_ptr<SpriteSheet> spriteSheet);
    
    glm::vec3 GetPosition();
    void SetPosition(const glm::vec3& position);
    
    void SetConfig(EmitterConfig* config);
    EmitterConfig* GetConfig();
    ParticleList& GetParticles();
    
private:
    static bool ParticleSortPredicate(const Particle& a, const Particle& b);
    ParticleList _Particles;
    
    ModifierList _Modifiers;
    
    std::shared_ptr<SpriteSheet> _SpriteSheet;
    EmitterConfig* _Config;
    glm::vec3 _Position;
    float _EmitCount;
    int _SpawnedParticles;
    
    IndexBuffer* _IndexBuffer;
    VertexBuffer* _VertexBuffer;
    
    bool _BufferDirty;
    int _MaxParticles;
    
    friend class ParticleModifier;
    friend class ParticleRenderer;
};
    
class ParticleModifier
{
public:
    ParticleModifier(ParticleEmitter* emitter);
    virtual ~ParticleModifier();
                     
    void Update(float time);
                     
protected:
    virtual void UpdateParticles(float time, ParticleEmitter::ParticleList& particles) = 0;
    
private:
    ParticleEmitter* _Emitter;
};
                                          
class ParticleAttractor : public ParticleModifier
{
public:
    ParticleAttractor(ParticleEmitter* emitter, const glm::vec3& position, float strength);
    virtual ~ParticleAttractor();
    
    virtual void UpdateParticles(float time, ParticleEmitter::ParticleList& particles);
    
private:
    glm::vec3 _Position;
    float _Strength;
};
    
class ParticleRenderer : public IRenderer
{
public:
    ParticleRenderer();
    ~ParticleRenderer();
    
    virtual void Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass);
};

}
