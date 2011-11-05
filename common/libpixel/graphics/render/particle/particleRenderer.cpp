#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/indexBuffer.h"
#include "libpixel/graphics/device/vertexBuffer.h"
#include "libpixel/graphics/render/particle/particleRenderer.h"
#include "libpixel/graphics/render/sprite/sprite.h"
#include "libpixel/logic/game.h"

using namespace libpixel;

ParticleRenderer::Particle::Particle(const ParticleEmitterConfig& config)
    : emitterConfig(&config)
{
    
}

ParticleRenderer::ParticleEmitterConfig::ParticleEmitterConfig()
{
    particlesPerUpdate = 1.f;
    
    initialScale = 1.f;
    life = 1.f;
    
//    sprite = "";
    
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
    gravityScale = 1.f;
}

ParticleRenderer::ParticleEmitter::ParticleEmitter()
    : _HasSpriteSheetOwnership(false)
{
    
}

ParticleRenderer::ParticleEmitter::~ParticleEmitter()
{
    
}

bool ParticleRenderer::ParticleEmitter::Load(const std::string& file)
{
    return false;
}

void ParticleRenderer::ParticleEmitter::LoadSpriteSheet(const std::string& file, bool createMips)
{
    if (_HasSpriteSheetOwnership && _SpriteSheet)
        delete _SpriteSheet;
    
    _HasSpriteSheetOwnership = true;
    _SpriteSheet = new SpriteSheet();
    _SpriteSheet->Load(file, createMips);
}

void ParticleRenderer::ParticleEmitter::SetSpriteSheet(SpriteSheet* spriteSheet, bool takeOwnership)
{
    if (_HasSpriteSheetOwnership && _SpriteSheet)
        delete _SpriteSheet;
    
    _HasSpriteSheetOwnership = takeOwnership;
    _SpriteSheet = spriteSheet;
}

Vec2 ParticleRenderer::ParticleEmitter::GetPosition()
{
    return _Position;
}

void ParticleRenderer::ParticleEmitter::SetPosition(const Vec2& position)
{
    _Position = position;
}

const ParticleRenderer::ParticleEmitterConfig& ParticleRenderer::ParticleEmitter::GetConfig()
{
    return _Config;
}

void ParticleRenderer::ParticleEmitter::SetConfig(const ParticleEmitterConfig& config)
{
    _Config = config;
    _Config.emitter = this;
}

ParticleRenderer::ParticleRenderer(int maxParticlesPerLayer)
{
    _MaxParticles = maxParticlesPerLayer;
    
    _IndexBuffer = libpixel::GraphicsDevice::Instance()->CreateIndexBuffer(libpixel::kBufferFormatStatic, _MaxParticles*6);
    _VertexBuffer = libpixel::GraphicsDevice::Instance()->CreateVertexBuffer(libpixel::kBufferFormatStatic, libpixel::kVertexFormat_P_XYZ_RGBA_UV, _MaxParticles*4);
}

ParticleRenderer::~ParticleRenderer()
{
    libpixel::GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    libpixel::GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
}

ParticleRenderer::ParticleEmitter* ParticleRenderer::CreateEmitter(RenderLayer* layer)
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
            const ParticleEmitterConfig& config = emitter->GetConfig();
            
            Particle particle(config);
            particle.position[0] = emitter->_Position[0] + config.minPosOffset[0] + (config.maxPosOffset[0]-config.minPosOffset[0]) * (float)rand()/(float)RAND_MAX;
            particle.position[1] = emitter->_Position[1] + config.minPosOffset[1] + (config.maxPosOffset[1]-config.minPosOffset[1]) * (float)rand()/(float)RAND_MAX;
            particle.rotation = config.minRotOffset + (config.maxRotOffset-config.minRotOffset) * (float)rand()/(float)RAND_MAX;
            particle.rotationVelocity = config.minRotVelocity + (config.maxRotVelocity-config.minRotVelocity) * (float)rand()/(float)RAND_MAX;
            particle.positionVelocity[0] = config.minPosVelocity[0] + (config.maxPosVelocity[0]-config.minPosVelocity[0]) * (float)rand()/(float)RAND_MAX;
            particle.positionVelocity[1] = config.minPosVelocity[1] + (config.maxPosVelocity[1]-config.minPosVelocity[1]) * (float)rand()/(float)RAND_MAX;
            particle.life = 0;
            particle.sprite = config.sprites[(int)(float)rand()/(float)RAND_MAX * config.sprites.size()];
            particle.totalLife = config.life;
            
            _Particles[emitterListIt->first].push_back(particle);
        }
    }
    
    for (ParticleListMap::iterator particleListIt = _Particles.begin(); particleListIt != _Particles.end(); ++particleListIt)
    {
        for (ParticleList::iterator it = particleListIt->second.begin(); it != particleListIt->second.end(); )
        {
            it->life += time;
            
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
    
    _IndexBuffer->Lock();
    _VertexBuffer->Lock();
    
    unsigned short* indexBuffer = _IndexBuffer->GetData();
    libpixel::Vertex_PXYZ_RGBA_UV* vertexBuffer = static_cast<libpixel::Vertex_PXYZ_RGBA_UV*>(_VertexBuffer->GetData());
    
    std::stable_sort(particleList.begin(), particleList.end(), &ParticleRenderer::ParticleSortPredicate);
    
    if (indexBuffer && vertexBuffer)
    {
        int index = 0;
        
        for (ParticleList::iterator it = particleList.begin(); it != particleList.end(); ++it)
        {
            float tween = it->life/it->emitterConfig->life;
            Vec4 color = it->emitterConfig->startColor + (it->emitterConfig->endColor-it->emitterConfig->startColor)*tween;
            
#ifdef LIBPIXEL_GRAPHICS_PREMULTIPLIED_ALPHA
            float alpha = color[3];
            color *= alpha;
            color[3] = alpha;
#endif
            
            Vec2 scale = it->emitterConfig->startScale + (it->emitterConfig->endScale-it->emitterConfig->startScale)*tween;
            
            vertexBuffer[0].position[0] = (cos(it->rotation[2])*scale[0])+it->position[0];
            vertexBuffer[0].position[1] = (sin(it->rotation[2])*scale[1])+it->position[1];
            vertexBuffer[0].position[2] = 0.f;
            vertexBuffer[1].position[0] = (cos(it->rotation[2]+M_PI_2)*scale[0])+it->position[0];
            vertexBuffer[1].position[1] = (sin(it->rotation[2]+M_PI_2)*scale[1])+it->position[1];
            vertexBuffer[1].position[2] = 0.f;
            vertexBuffer[2].position[0] = (cos(it->rotation[2]-M_PI_2)*scale[0])+it->position[0];
            vertexBuffer[2].position[1] = (sin(it->rotation[2]-M_PI_2)*scale[1])+it->position[1];
            vertexBuffer[2].position[2] = 0.f;
            vertexBuffer[3].position[0] = (cos(it->rotation[2]+M_PI)*scale[0])+it->position[0];
            vertexBuffer[3].position[1] = (sin(it->rotation[2]+M_PI)*scale[1])+it->position[1];
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
            
            libpixel::Sprite* sprite = it->emitterConfig->emitter->_SpriteSheet->GetSprite(it->sprite);
            
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
            
            indexBuffer[0] = 0 + index;
            indexBuffer[1] = 1 + index;
            indexBuffer[2] = 2 + index;
            indexBuffer[3] = 1 + index;
            indexBuffer[4] = 2 + index;
            indexBuffer[5] = 3 + index;
            
            indexBuffer += 6;
            index += 4;
            
            sprite->_Sheet->_Texture->Bind();
        }
    }
    
    _IndexBuffer->Unlock();
    _VertexBuffer->Unlock();
    
    if (indexBuffer && vertexBuffer)
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
        glDrawElements(GL_TRIANGLES, particleList.size()*6, GL_UNSIGNED_SHORT, (void*)0);
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
