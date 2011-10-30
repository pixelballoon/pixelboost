#pragma once

#include <vector>

#include "libpixel/math/maths.h"

class ParticleRenderer
{
public:
    ParticleRenderer();
    ~ParticleRenderer();
    
    void AddSystem(const std::string& file);
    
public:
    struct Particl
    {
        
    };
    
    class ParticleEmitter
    {
    public:
        ParticleEmitter();
        ~ParticleEmitter();
        
        Vec2 _Position;
        
    private:
    };
    
    class ParticleSystem
    {
    public:
        ParticleSystem();
        ~ParticleSystem();
        
        void AddEmitter(const std::string& emitter);
        
    private:
        typedef std::vector<Particle> ParticleList;
        typedef std::vector<ParticleEmitter*> EmitterList;
        
        EmitterList _Emitters;
        ParticleList _Particles;
    };
    
private:
    typedef std::vector<ParticleSystem*> SystemList;
    SystemList _Systems;
};
