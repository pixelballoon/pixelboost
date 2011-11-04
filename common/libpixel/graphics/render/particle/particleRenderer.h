#pragma once

#include <string>
#include <vector>

#include "libpixel/graphics/render/common/irenderer.h"
#include "libpixel/math/maths.h"

namespace libpixel
{
    
class RenderLayer;
class SpriteSheet;
    
class ParticleRenderer : IRenderer
{
public:
    class ParticleEmitter;
    class ParticleEmitterConfig;
    
public:
    ParticleRenderer(int maxParticlesPerLayer=200);
    ~ParticleRenderer();
    
    ParticleEmitter* CreateEmitter(RenderLayer* layer);
    void DestroyEmitter(ParticleEmitter* emitter);
    
    virtual void Update(float time);
    virtual void Render(RenderLayer* layer);
    
public:
    struct Particle
    {
        Particle(const ParticleEmitterConfig& config);
        
        std::string sprite;
        
        const ParticleEmitterConfig* emitterConfig;
        
        float life;
        float totalLife;
        
        Vec3 rotation;
        Vec2 position;
        
        Vec3 rotationVelocity;
        Vec2 positionVelocity;
    };
    
    struct ParticleEmitterConfig
    {
        ParticleEmitterConfig();
        
        ParticleEmitter* emitter;
        
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
        float gravityScale;
        
        typedef std::vector<std::string> SpriteList;
        SpriteList sprites;
    };
    
    class ParticleEmitter
    {
    protected:
        ParticleEmitter();
        ~ParticleEmitter();
        
    public:
        bool Load(const std::string& file);
        
        void LoadSpriteSheet(const std::string& file, bool createMips);
        void SetSpriteSheet(SpriteSheet* spriteSheet, bool takeOwnership);
        
        Vec2 GetPosition();
        void SetPosition(const Vec2& position);
        
        const ParticleEmitterConfig& GetConfig();
        void SetConfig(const ParticleEmitterConfig& config);
        
    private:
        SpriteSheet* _SpriteSheet;
        bool _HasSpriteSheetOwnership;
        
        ParticleEmitterConfig _Config;
        Vec2 _Position;
        
        friend class ParticleRenderer;
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