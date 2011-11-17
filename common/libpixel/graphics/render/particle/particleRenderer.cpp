#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/indexBuffer.h"
#include "libpixel/graphics/device/vertexBuffer.h"
#include "libpixel/graphics/render/particle/particleRenderer.h"
#include "libpixel/graphics/render/sprite/sprite.h"
#include "libpixel/logic/game.h"

using namespace libpixel;

ParticleRenderer::Particle::Particle(ParticleEmitter::Config* config)
    : emitterConfig(config)
{
    emitterConfig->refCount++;
}

ParticleRenderer::Particle::Particle(const Particle& rhs)
{
    Assign(rhs);
}

ParticleRenderer::Particle::~Particle()
{
    emitterConfig->refCount--;
    
    if (emitterConfig->refCount == 0)
    {
        delete emitterConfig;
    }
}

ParticleRenderer::Particle& ParticleRenderer::Particle::operator=(const Particle& rhs)
{
    Assign(rhs);
    
    return *this;
}

void ParticleRenderer::Particle::Assign(const Particle& rhs)
{
    emitterConfig = rhs.emitterConfig;
    emitterConfig->refCount++;
    
    sprite = rhs.sprite;
    
    life = rhs.life;
    totalLife = rhs.totalLife;
    
    rotation = rhs.rotation;
    position = rhs.position;
    
    rotationVelocity = rhs.rotationVelocity;
    positionVelocity = rhs.positionVelocity;
}

ParticleEmitter::Config::Config()
{
    hasSpriteSheetOwnership = false;
    spriteSheet = 0;
    
    refCount = 1;
    
    particlesPerUpdate = 1.f;
    
    initialScale = 1.f;
    life = 1.f;
    
    startScale = Vec2(1.f, 1.f);
    endScale = Vec2(1.f, 1.f);
    startColor = Vec4(1.f, 1.f, 1.f, 1.f);
    endColor = Vec4(1.f, 1.f, 1.f, 1.f);
    minPosOffset = Vec2(0.f, 0.f);
    maxPosOffset = Vec2(0.f, 0.f);
    minRotOffset = Vec3(0.f, 0.f, 0.f);
    maxRotOffset = Vec3(0.f, 0.f, 0.f);
    minRotVelocity = Vec3(0.f, 0.f, 0.f);
    maxRotVelocity = Vec3(0.f, 0.f, 0.f);
    minPosVelocity = Vec2(0.f, 0.f);
    maxPosVelocity = Vec2(0.f, 0.f);
    gravity = Vec2(0.f, 0.f);
}

ParticleEmitter::Config::~Config()
{
    if (hasSpriteSheetOwnership && spriteSheet)
    {
        delete spriteSheet;
    }
}

ParticleEmitter::ParticleEmitter()
{
    _Config = new Config();
    _EmitCount = 0.f;
}

ParticleEmitter::~ParticleEmitter()
{
    _Config->refCount--;
    
    if (_Config->refCount == 0)
        delete _Config;
}

bool ParticleEmitter::Load(const std::string& file)
{
    return false;
}

void ParticleEmitter::LoadSpriteSheet(const std::string& file, bool createMips)
{
    if (_Config->hasSpriteSheetOwnership && _Config->spriteSheet)
        delete _Config->spriteSheet;
    
    _Config->hasSpriteSheetOwnership = true;
    _Config->spriteSheet = new SpriteSheet();
    _Config->spriteSheet->Load(file, createMips);
}

void ParticleEmitter::SetSpriteSheet(SpriteSheet* spriteSheet, bool takeOwnership)
{
    if (_Config->hasSpriteSheetOwnership && _Config->spriteSheet)
        delete _Config->spriteSheet;
    
    _Config->hasSpriteSheetOwnership = takeOwnership;
    _Config->spriteSheet = spriteSheet;
}

Vec2 ParticleEmitter::GetPosition()
{
    return _Position;
}

void ParticleEmitter::SetPosition(const Vec2& position)
{
    _Position = position;
}

ParticleEmitter::Config& ParticleEmitter::GetConfig()
{
    return *_Config;
}

ParticleRenderer::ParticleRenderer(int maxParticlesPerLayer)
{
    _MaxParticles = maxParticlesPerLayer;
    
    _IndexBuffer = libpixel::GraphicsDevice::Instance()->CreateIndexBuffer(libpixel::kBufferFormatStatic, _MaxParticles*6);
    _VertexBuffer = libpixel::GraphicsDevice::Instance()->CreateVertexBuffer(libpixel::kBufferFormatDynamic, libpixel::kVertexFormat_P_XYZ_RGBA_UV, _MaxParticles*4);
    
    _IndexBuffer->Lock();
    unsigned short* indexBuffer = _IndexBuffer->GetData();
    for (int i=0; i<_MaxParticles; i++)
    {
        indexBuffer[0] = (i*4);
        indexBuffer[1] = (i*4) + 1;
        indexBuffer[2] = (i*4) + 2;
        indexBuffer[3] = (i*4) + 1;
        indexBuffer[4] = (i*4) + 2;
        indexBuffer[5] = (i*4) + 3;
        
        indexBuffer += 6;
    }
    _IndexBuffer->Unlock();
}

ParticleRenderer::~ParticleRenderer()
{
    libpixel::GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    libpixel::GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
}

ParticleEmitter* ParticleRenderer::CreateEmitter(RenderLayer* layer)
{
    ParticleEmitter* emitter = new ParticleEmitter();
    _Emitters[layer].push_back(emitter);
    return emitter;
}
    
void ParticleRenderer::DestroyEmitter(ParticleEmitter* emitter)
{
    for (EmitterListMap::iterator emitterListIt = _Emitters.begin(); emitterListIt != _Emitters.end(); ++emitterListIt)
    {
        for (EmitterList::iterator emitterIt = emitterListIt->second.begin(); emitterIt != emitterListIt->second.end(); ++emitterIt)
        {
            if (emitter == *emitterIt)
            {
                emitterListIt->second.erase(emitterIt);
                delete emitter;
                return;
            }
        }
    }
}
    
void ParticleRenderer::Update(float time)
{
    for (EmitterListMap::iterator emitterListIt = _Emitters.begin(); emitterListIt != _Emitters.end(); ++emitterListIt)
    {
        for (EmitterList::iterator it = emitterListIt->second.begin(); it != emitterListIt->second.end(); ++it)
        {
            if (_Particles[emitterListIt->first].size() >= _MaxParticles)
                break;
            
            ParticleEmitter* emitter = *it;
            ParticleEmitter::Config& config = emitter->GetConfig();
            
            emitter->_EmitCount += config.particlesPerUpdate;
            
            if (!config.sprites.size())
                continue;
            
            while (emitter->_EmitCount >= 1.f)
            {
                Particle particle(&config);
                particle.position[0] = emitter->_Position[0] + config.minPosOffset[0] + (config.maxPosOffset[0]-config.minPosOffset[0]) * (float)rand()/(float)RAND_MAX;
                particle.position[1] = emitter->_Position[1] + config.minPosOffset[1] + (config.maxPosOffset[1]-config.minPosOffset[1]) * (float)rand()/(float)RAND_MAX;
                particle.rotation = config.minRotOffset + (config.maxRotOffset-config.minRotOffset) * (float)rand()/(float)RAND_MAX;
                particle.rotationVelocity = config.minRotVelocity + (config.maxRotVelocity-config.minRotVelocity) * (float)rand()/(float)RAND_MAX;
                particle.positionVelocity[0] = config.minPosVelocity[0] + (config.maxPosVelocity[0]-config.minPosVelocity[0]) * (float)rand()/(float)RAND_MAX;
                particle.positionVelocity[1] = config.minPosVelocity[1] + (config.maxPosVelocity[1]-config.minPosVelocity[1]) * (float)rand()/(float)RAND_MAX;
                particle.life = 0;
                particle.sprite = config.sprites[Min((int)((float)rand()/(float)RAND_MAX * config.sprites.size()), (int)config.sprites.size()-1)];
                particle.totalLife = config.life;

                _Particles[emitterListIt->first].push_back(particle);
                
                emitter->_EmitCount -= 1.f;
            }
        }
    }
    
    for (ParticleListMap::iterator particleListIt = _Particles.begin(); particleListIt != _Particles.end(); ++particleListIt)
    {
        for (ParticleList::iterator it = particleListIt->second.begin(); it != particleListIt->second.end(); )
        {
            it->life += time;
            
            it->positionVelocity += it->emitterConfig->gravity;
            
            it->rotation += it->rotationVelocity;
            it->position += it->positionVelocity;
            
            if (it->life > it->totalLife)
            {
                it = particleListIt->second.erase(it);
            } else {
                it++;
            }
        }
    }
}

void ParticleRenderer::Render(RenderLayer* layer)
{
    ParticleList& particleList = _Particles[layer];

    if (!particleList.size())
        return;
    
    _VertexBuffer->Lock();
    
    libpixel::Vertex_PXYZ_RGBA_UV* vertexBuffer = static_cast<libpixel::Vertex_PXYZ_RGBA_UV*>(_VertexBuffer->GetData());
    
    std::stable_sort(particleList.begin(), particleList.end(), &ParticleRenderer::ParticleSortPredicate);
    
    if (vertexBuffer)
    {
        for (ParticleList::iterator it = particleList.begin(); it != particleList.end(); ++it)
        {
            float tween = it->life/it->emitterConfig->life;
            Vec4 color = it->emitterConfig->startColor + (it->emitterConfig->endColor-it->emitterConfig->startColor)*tween;
            
#ifdef LIBPIXEL_GRAPHICS_PREMULTIPLIED_ALPHA
            float alpha = color[3];
            color *= alpha;
            color[3] = alpha;
#endif
            libpixel::Sprite* sprite = it->emitterConfig->spriteSheet->GetSprite(it->sprite);
            
            Vec2 scale = it->emitterConfig->startScale + (it->emitterConfig->endScale-it->emitterConfig->startScale)*tween;
            Vec2 size = sprite->_Dimension * scale;
            
            vertexBuffer[0].position[0] = (cos(it->rotation[2])*size[0])+it->position[0];
            vertexBuffer[0].position[1] = (sin(it->rotation[2])*size[1])+it->position[1];
            vertexBuffer[0].position[2] = 0.f;
            vertexBuffer[1].position[0] = (cos(it->rotation[2]+M_PI_2)*size[0])+it->position[0];
            vertexBuffer[1].position[1] = (sin(it->rotation[2]+M_PI_2)*size[1])+it->position[1];
            vertexBuffer[1].position[2] = 0.f;
            vertexBuffer[2].position[0] = (cos(it->rotation[2]-M_PI_2)*size[0])+it->position[0];
            vertexBuffer[2].position[1] = (sin(it->rotation[2]-M_PI_2)*size[1])+it->position[1];
            vertexBuffer[2].position[2] = 0.f;
            vertexBuffer[3].position[0] = (cos(it->rotation[2]+M_PI)*size[0])+it->position[0];
            vertexBuffer[3].position[1] = (sin(it->rotation[2]+M_PI)*size[1])+it->position[1];
            vertexBuffer[3].position[2] = 0.f;
            
            vertexBuffer[0].color[0] = color[0];
            vertexBuffer[0].color[1] = color[1];
            vertexBuffer[0].color[2] = color[2];
            vertexBuffer[0].color[3] = color[3];
            vertexBuffer[1].color[0] = color[0];
            vertexBuffer[1].color[1] = color[1];
            vertexBuffer[1].color[2] = color[2];
            vertexBuffer[1].color[3] = color[3];
            vertexBuffer[2].color[0] = color[0];
            vertexBuffer[2].color[1] = color[1];
            vertexBuffer[2].color[2] = color[2];
            vertexBuffer[2].color[3] = color[3];
            vertexBuffer[3].color[0] = color[0];
            vertexBuffer[3].color[1] = color[1];
            vertexBuffer[3].color[2] = color[2];
            vertexBuffer[3].color[3] = color[3];
            
            if (!sprite->_Rotated)
            {
                Vec2 min = sprite->_Position;
                Vec2 max = sprite->_Position + sprite->_Size;
                
                vertexBuffer[0].uv[0] = min[0];
                vertexBuffer[0].uv[1] = min[1],
                vertexBuffer[1].uv[0] = max[0];
                vertexBuffer[1].uv[1] = min[1];
                vertexBuffer[2].uv[0] = min[0];
                vertexBuffer[2].uv[1] = max[1];
                vertexBuffer[3].uv[0] = max[0];
                vertexBuffer[3].uv[1] = max[1];
            } else {
                Vec2 min = sprite->_Position + Vec2(sprite->_Size[1], 0);
                Vec2 max = sprite->_Position + Vec2(0, sprite->_Size[0]);
                
                vertexBuffer[0].uv[0] = max[0];
                vertexBuffer[0].uv[1] = min[1];
                vertexBuffer[1].uv[0] = min[0];
                vertexBuffer[1].uv[1] = min[1];
                vertexBuffer[2].uv[0] = min[0];
                vertexBuffer[2].uv[1] = max[1];
                vertexBuffer[3].uv[0] = max[0];
                vertexBuffer[3].uv[1] = max[1];
            }
            
            vertexBuffer += 4;
            
            sprite->_Sheet->_Texture->Bind();
        }
    }
    
    _VertexBuffer->Unlock(particleList.size()*4);
    
    if (vertexBuffer && particleList.size())
    {
        _IndexBuffer->Bind();
        _VertexBuffer->Bind();
        
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
#ifdef LIBPIXEL_GRAPHICS_PREMULTIPLIED_ALPHA
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#else
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, particleList.size()*6);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }
}

bool ParticleRenderer::ParticleSortPredicate(const Particle& a, const Particle& b)
{
    return true;
}
