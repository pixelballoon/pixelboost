#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/graphics/render/common/irenderer.h"
#include "pixelboost/math/maths.h"
#include "pixelboost/misc/pointer.h"

namespace pixelboost
{
    
class IndexBuffer;
class Renderer;
class RenderLayer;
class SpriteSheet;
class VertexBuffer;
    
class ParticleEmitter
{
protected:
    ParticleEmitter();
    ~ParticleEmitter();
    
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

public:
    bool Load(const std::string& file);
    
    void LoadSpriteSheet(const std::string& file, bool createMips);
    void SetSpriteSheet(std::shared_ptr<SpriteSheet> spriteSheet);
    
    Vec2 GetPosition();
    void SetPosition(const Vec2& position);
    
    Config& GetConfig();
    
private:
    Config* _Config;
    Vec2 _Position;
    float _EmitCount;
    
    friend class ParticleRenderer;
};
    
class ParticleRenderer : public IRenderer
{
public:
    ParticleRenderer(Renderer* renderer, int maxParticlesPerLayer=200);
    ~ParticleRenderer();
    
    ParticleEmitter* CreateEmitter(RenderLayer* layer);
    void DestroyEmitter(ParticleEmitter* emitter);
    
    virtual void Update(float time);
    virtual void Render(RenderLayer* layer);
    
public:
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
    
private:
    static bool ParticleSortPredicate(const Particle& a, const Particle& b);
    
    typedef std::vector<Particle> ParticleList;
    typedef std::vector<ParticleEmitter*> EmitterList;
    
    typedef std::map<RenderLayer*, EmitterList> EmitterListMap;
    typedef std::map<RenderLayer*, ParticleList> ParticleListMap;
    
    EmitterListMap _Emitters;
    ParticleListMap _Particles;
    
    IndexBuffer* _IndexBuffer;
    VertexBuffer* _VertexBuffer;
    
    int _MaxParticles;
};

}