#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/math/maths.h"
#include "pixelboost/misc/pointer.h"

namespace pb
{
    
class IndexBuffer;
class ParticleModifier;
class RenderLayer;
class SpriteSheet;
class VertexBuffer;
    
class ParticleEmitter
{
protected:
    ParticleEmitter(int maxParticles=50);
    ~ParticleEmitter();
    
    void Update(float time);
    void Render();
    
public:
    struct Config
    {
        Config();
        ~Config();
        
        int refCount;
        
        float particlesPerUpdate;
        
        float initialScale;
        float life;
        
        Vec2 startScale;
        Vec2 endScale;
        Vec4 startColor;
        Vec4 endColor;
        Vec2 minPosOffset;
        Vec2 maxPosOffset;
        Vec3 minRotOffset;
        Vec3 maxRotOffset;
        Vec3 minRotVelocity;
        Vec3 maxRotVelocity;
        Vec2 minPosVelocity;
        Vec2 maxPosVelocity;
        Vec2 gravity;
        
        typedef std::vector<std::string> SpriteList;
        SpriteList sprites;
        
    private:
        std::shared_ptr<SpriteSheet> spriteSheet;
        
        friend class ParticleEmitter;
        friend class ParticleRenderer;
    };
    
private:
    struct Particle
    {
        Particle(ParticleEmitter::Config* config);
        Particle(const Particle& particle);
        ~Particle();
        
        Particle& operator=(const Particle& rhs);
        
        std::string sprite;
        
        ParticleEmitter::Config* emitterConfig;
        
        float life;
        float totalLife;
        
        Vec3 rotation;
        Vec2 position;
        
        Vec3 rotationVelocity;
        Vec2 positionVelocity;
        
    private:
        void Assign(const Particle& rhs);
    };

public:
    typedef std::vector<ParticleModifier*> ModifierList;
    typedef std::vector<Particle> ParticleList;
    
    void AddModifier(ParticleModifier* modifier);
    
    bool Load(const std::string& file);
    
    void LoadSpriteSheet(const std::string& file, bool createMips);
    void SetSpriteSheet(std::shared_ptr<SpriteSheet> spriteSheet);
    
    Vec2 GetPosition();
    void SetPosition(const Vec2& position);
    
    Config& GetConfig();
    ParticleList& GetParticles();
    
private:
    static bool ParticleSortPredicate(const Particle& a, const Particle& b);
    ParticleList _Particles;
    
    ModifierList _Modifiers;
    
    Config* _Config;
    Vec2 _Position;
    float _EmitCount;
    
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
    ParticleAttractor(ParticleEmitter* emitter, const Vec2& position, float strength);
    virtual ~ParticleAttractor();
    
    virtual void UpdateParticles(float time, ParticleEmitter::ParticleList& particles);
    
private:
    Vec2 _Position;
    float _Strength;
};
    
class ParticleRenderer : public IRenderer
{
public:
    ParticleRenderer();
    ~ParticleRenderer();
    
    ParticleEmitter* CreateEmitter(RenderLayer* layer, int maxParticles=50);
    void DestroyEmitter(ParticleEmitter* emitter);
    
    virtual void Update(float time);
    virtual void Render(RenderLayer* layer);
    
private:
    typedef std::vector<ParticleEmitter*> EmitterList;
    typedef std::map<RenderLayer*, EmitterList> EmitterListMap;
    
    EmitterListMap _Emitters;
};

}
