#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/effect/effect.h"
#include "pixelboost/graphics/effect/manager.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/particle/particleRenderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"

using namespace pb;


ParticleRenderable::ParticleRenderable(Uid entityId, int maxParticles)
    : Renderable(entityId)
{
    _Emitter = new ParticleEmitter(maxParticles);
}

ParticleRenderable::~ParticleRenderable()
{
    delete _Emitter;
}

Uid ParticleRenderable::GetRenderableType()
{
    return TypeHash("particle");
}

void ParticleRenderable::CalculateBounds()
{
    SetBounds(BoundingSphere());
}

void ParticleRenderable::CalculateWorldMatrix()
{
    SetWorldMatrix(glm::mat4x4());
}

Effect* ParticleRenderable::GetEffect()
{
    Effect* baseEffect = Renderable::GetEffect();
    if (baseEffect)
        return baseEffect;
    
    return Renderer::Instance()->GetEffectManager()->GetEffect("/default/effects/particle.fx");
}

ParticleEmitter* ParticleRenderable::GetEmitter()
{
    return _Emitter;
}

ParticleEmitter::Particle::Particle(EmitterConfig* config)
    : emitterConfig(config)
{
    
}

ParticleEmitter::Particle::Particle(const Particle& rhs)
    : emitterConfig(rhs.emitterConfig)
{
    Assign(rhs);
}

ParticleEmitter::Particle::~Particle()
{
    
}

ParticleEmitter::Particle& ParticleEmitter::Particle::operator=(const Particle& rhs)
{
    Assign(rhs);
    
    return *this;
}

void ParticleEmitter::Particle::Assign(const Particle& rhs)
{
    emitterConfig = rhs.emitterConfig;

    sprite = rhs.sprite;
    
    life = rhs.life;
    totalLife = rhs.totalLife;
    
    rotation = rhs.rotation;
    position = rhs.position;
    
    rotationVelocity = rhs.rotationVelocity;
    positionVelocity = rhs.positionVelocity;
}

EmitterConfig::EmitterConfig()
{
    emitCount = -1;
    particlesPerUpdate = 1.f;
    
    initialScale = 1.f;
    life = 1.f;
    
    startScale = glm::vec2(1.f, 1.f);
    endScale = glm::vec2(1.f, 1.f);
    startColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    endColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    minPosOffset = glm::vec3(0.f, 0.f, 0.f);
    maxPosOffset = glm::vec3(0.f, 0.f, 0.f);
    minRotOffset = glm::vec3(0.f, 0.f, 0.f);
    maxRotOffset = glm::vec3(0.f, 0.f, 0.f);
    minRotVelocity = glm::vec3(0.f, 0.f, 0.f);
    maxRotVelocity = glm::vec3(0.f, 0.f, 0.f);
    minPosVelocity = glm::vec3(0.f, 0.f, 0.f);
    maxPosVelocity = glm::vec3(0.f, 0.f, 0.f);
    gravity = glm::vec3(0.f, 0.f, 0.f);
}

EmitterConfig::~EmitterConfig()
{

}

ParticleEmitter::ParticleEmitter(int maxParticles)
    : _Config(0)
    , _EmitCount(0)
    , _MaxParticles(maxParticles)
    , _SpawnedParticles(0)
{
    _IndexBuffer = pb::GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, _MaxParticles*6);
    _VertexBuffer = pb::GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P_XYZ_RGBA_UV, _MaxParticles*4);
    
    _IndexBuffer->Lock();
    unsigned short* indexBuffer = _IndexBuffer->GetData();
    for (int i=0; i<_MaxParticles; i++)
    {
        indexBuffer[0] = (i*4);
        indexBuffer[1] = (i*4) + 1;
        indexBuffer[2] = (i*4) + 2;
        indexBuffer[3] = (i*4) + 0;
        indexBuffer[4] = (i*4) + 2;
        indexBuffer[5] = (i*4) + 3;
        
        indexBuffer += 6;
    }
    _IndexBuffer->Unlock();
}

ParticleEmitter::~ParticleEmitter()
{
    if (_Config)
    {
        delete _Config;
    }
    
    for (ModifierList::iterator it = _Modifiers.begin(); it != _Modifiers.end(); ++it)
    {
        delete *it;
    }
    
    pb::GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    pb::GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);    
}

void ParticleEmitter::Update(float time)
{
    if (!_Config || !_Config->sprites.size())
        return;
    
    for (ModifierList::iterator it = _Modifiers.begin(); it != _Modifiers.end(); ++it)
    {
        (*it)->Update(time);
    }
    
    if (_Config->emitCount == -1 || _SpawnedParticles < _Config->emitCount)
        _EmitCount += _Config->particlesPerUpdate;
    
    while (_EmitCount >= 1.f)
    {
        if (_Particles.size() < _MaxParticles)
        {
            _SpawnedParticles++;
            
            Particle particle(_Config);
            particle.position.x = _Position.x + _Config->minPosOffset.x + (_Config->maxPosOffset.x-_Config->minPosOffset.x) * (float)rand()/(float)RAND_MAX;
            particle.position.y = _Position.y + _Config->minPosOffset.y + (_Config->maxPosOffset.y-_Config->minPosOffset.y) * (float)rand()/(float)RAND_MAX;
            particle.position.z = _Position.z + _Config->minPosOffset.z + (_Config->maxPosOffset.z-_Config->minPosOffset.z) * (float)rand()/(float)RAND_MAX;
            particle.rotation = _Config->minRotOffset + (_Config->maxRotOffset-_Config->minRotOffset) * (float)rand()/(float)RAND_MAX;
            particle.rotationVelocity = _Config->minRotVelocity + (_Config->maxRotVelocity-_Config->minRotVelocity) * (float)rand()/(float)RAND_MAX;
            particle.positionVelocity.x = _Config->minPosVelocity.x + (_Config->maxPosVelocity.x-_Config->minPosVelocity.x) * (float)rand()/(float)RAND_MAX;
            particle.positionVelocity.y = _Config->minPosVelocity.y + (_Config->maxPosVelocity.y-_Config->minPosVelocity.y) * (float)rand()/(float)RAND_MAX;
            particle.positionVelocity.z = _Config->minPosVelocity.z + (_Config->maxPosVelocity.z-_Config->minPosVelocity.z) * (float)rand()/(float)RAND_MAX;
            particle.life = 0;
            particle.sprite = _Config->sprites[glm::min((int)((float)rand()/(float)RAND_MAX * _Config->sprites.size()), (int)_Config->sprites.size()-1)];
            particle.totalLife = _Config->life;
            
            _Particles.push_back(particle);
        }
        
        _EmitCount -= 1.f;
    }
    
    for (ParticleList::iterator it = _Particles.begin(); it != _Particles.end(); )
    {
        it->life += time;
        
        it->positionVelocity += it->emitterConfig->gravity;
        
        it->rotation += it->rotationVelocity;
        it->position += it->positionVelocity;
        
        if (it->life > it->totalLife)
        {
            it = _Particles.erase(it);
        } else {
            it++;
        }
    }
}

void ParticleEmitter::Render(Viewport* viewport, EffectPass* effectPass)
{
    if (!_Config || !_Particles.size())
        return;
    
    _VertexBuffer->Lock();
    
    pb::Vertex_PXYZ_RGBA_UV* vertexBuffer = static_cast<pb::Vertex_PXYZ_RGBA_UV*>(_VertexBuffer->GetData());
    
    if (vertexBuffer)
    {
        for (ParticleList::iterator it = _Particles.begin(); it != _Particles.end(); ++it)
        {
            float tween = it->life/it->emitterConfig->life;
            glm::vec4 color = it->emitterConfig->startColor + (it->emitterConfig->endColor-it->emitterConfig->startColor)*tween;
            
#ifdef PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
            float alpha = color[3];
            color *= alpha;
            color[3] = alpha;
#endif
            pb::Sprite* sprite = it->emitterConfig->spriteSheet->GetSprite(it->sprite);
            
            glm::vec2 scale = it->emitterConfig->startScale + (it->emitterConfig->endScale-it->emitterConfig->startScale)*tween;
            glm::vec2 size = sprite->_Size * scale;
            
            glm::mat4x4 transform = glm::translate(glm::mat4x4(), it->position);
            transform = glm::scale(transform, glm::vec3(scale, 1));
            transform = glm::rotate(transform, it->rotation.x, glm::vec3(1,0,0));
            transform = glm::rotate(transform, it->rotation.y, glm::vec3(0,1,0));
            transform = glm::rotate(transform, it->rotation.y, glm::vec3(0,0,1));
            
            glm::vec4 a = transform * glm::vec4(-0.5, -0.5, 0, 1);
            glm::vec4 b = transform * glm::vec4(-0.5, 0.5, 0, 1);
            glm::vec4 c = transform * glm::vec4(0.5, 0.5, 0, 1);
            glm::vec4 d = transform * glm::vec4(0.5, -0.5, 0, 1);
            
            vertexBuffer[0].position[0] = a.x;
            vertexBuffer[0].position[1] = a.y;
            vertexBuffer[0].position[2] = a.z;
            vertexBuffer[1].position[0] = b.x;
            vertexBuffer[1].position[1] = b.y;
            vertexBuffer[1].position[2] = b.z;
            vertexBuffer[2].position[0] = c.x;
            vertexBuffer[2].position[1] = c.y;
            vertexBuffer[2].position[2] = c.z;
            vertexBuffer[3].position[0] = d.x;
            vertexBuffer[3].position[1] = d.y;
            vertexBuffer[3].position[2] = d.z;
            
            /*
            float cosRot = cos(((-it->rotation[2]-90.f)/180.f)*M_PI);
            float sinRot = sin(((-it->rotation[2]-90.f)/180.f)*M_PI);
            
            vertexBuffer[0].position[0] = -size[1] * cosRot + size[0] * sinRot + it->position[0];
            vertexBuffer[0].position[1] = size[1] * sinRot + size[0] * cosRot + it->position[1];
            vertexBuffer[0].position[2] = 0.f;
            vertexBuffer[1].position[0] = size[1] * cosRot + size[0] * sinRot + it->position[0];
            vertexBuffer[1].position[1] = -size[1] * sinRot + size[0] * cosRot + it->position[1];
            vertexBuffer[1].position[2] = 0.f;
            vertexBuffer[2].position[0] = size[1] * cosRot - size[0] * sinRot + it->position[0];
            vertexBuffer[2].position[1] = -size[1] * sinRot - size[0] * cosRot + it->position[1];
            vertexBuffer[2].position[2] = 0.f;
            vertexBuffer[3].position[0] = -size[1] * cosRot - size[0] * sinRot + it->position[0];
            vertexBuffer[3].position[1] = size[1] * sinRot - size[0] * cosRot + it->position[1];
            vertexBuffer[3].position[2] = 0.f;
             */
            
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
                glm::vec2 min = sprite->_UvPosition;
                glm::vec2 max = sprite->_UvPosition + sprite->_UvSize;
                
                vertexBuffer[0].uv[0] = min[0];
                vertexBuffer[0].uv[1] = max[1],
                vertexBuffer[1].uv[0] = min[0];
                vertexBuffer[1].uv[1] = min[1];
                vertexBuffer[2].uv[0] = max[0];
                vertexBuffer[2].uv[1] = min[1];
                vertexBuffer[3].uv[0] = max[0];
                vertexBuffer[3].uv[1] = max[1];
            } else {
                glm::vec2 min = sprite->_UvPosition + glm::vec2(sprite->_UvSize[1], 0);
                glm::vec2 max = sprite->_UvPosition + glm::vec2(0, sprite->_UvSize[0]);
                
                vertexBuffer[0].uv[0] = max[0];
                vertexBuffer[0].uv[1] = max[1];
                vertexBuffer[1].uv[0] = min[0];
                vertexBuffer[1].uv[1] = max[1];
                vertexBuffer[2].uv[0] = min[0];
                vertexBuffer[2].uv[1] = min[1];
                vertexBuffer[3].uv[0] = max[0];
                vertexBuffer[3].uv[1] = min[1];
            }
            
            vertexBuffer += 4;
        }
        
        GraphicsDevice::Instance()->BindTexture(_Config->spriteSheet->_Texture);

        _VertexBuffer->Unlock(_Particles.size()*4);
    
        GraphicsDevice::Instance()->BindIndexBuffer(_IndexBuffer);
        GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
        
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);

#ifndef PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
        GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendSourceAlpha, GraphicsDevice::kBlendOneMinusSourceAlpha);
#else
        GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
#endif
        
        effectPass->GetShaderProgram()->SetUniform("diffuseTexture", 0);
        
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, _Particles.size()*6);
        
        GraphicsDevice::Instance()->BindIndexBuffer(0);
        GraphicsDevice::Instance()->BindVertexBuffer(0);
        
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    }
}

void ParticleEmitter::AddModifier(ParticleModifier* modifier)
{
    _Modifiers.push_back(modifier);
}

bool ParticleEmitter::IsFinished()
{
    if (!_Config || _Config->emitCount == -1)
        return false;
    
    return (_SpawnedParticles >= _Config->emitCount && _Particles.size() == 0);
}

int ParticleEmitter::GetNumParticles()
{
    return _Particles.size();
}

void ParticleEmitter::ResetSpawnCount()
{
    _SpawnedParticles = 0;
}

void ParticleEmitter::LoadSpriteSheet(FileLocation location, const std::string& file, bool createMips)
{
    if (!_Config)
        return;
    
    _Config->spriteSheet = SpriteSheet::Create();
    _Config->spriteSheet->LoadSheet(location, file, createMips);
}

void ParticleEmitter::SetSpriteSheet(std::shared_ptr<SpriteSheet> spriteSheet)
{
    if (!_Config)
        return;
    
    _Config->spriteSheet = spriteSheet;
}

glm::vec3 ParticleEmitter::GetPosition()
{
    return _Position;
}

void ParticleEmitter::SetPosition(const glm::vec3& position)
{
    _Position = position;
}

void ParticleEmitter::SetConfig(EmitterConfig* config)
{
    if (_Config)
    {
        delete _Config;
        _Config = 0;
    }
    
    _Config = config;
}

EmitterConfig* ParticleEmitter::GetConfig()
{
    return _Config;
}

ParticleEmitter::ParticleList& ParticleEmitter::GetParticles()
{
    _BufferDirty = true;
    return _Particles;
}

ParticleModifier::ParticleModifier(ParticleEmitter* emitter)
    : _Emitter(emitter)
{
    
}

ParticleModifier::~ParticleModifier()
{
    
}

void ParticleModifier::Update(float time)
{
    UpdateParticles(time, _Emitter->_Particles);
}

ParticleAttractor::ParticleAttractor(ParticleEmitter* emitter, const glm::vec3& position, float strength)
    : ParticleModifier(emitter)
    , _Position(position)
    , _Strength(strength)
{
    
}

ParticleAttractor::~ParticleAttractor()
{
    
}

void ParticleAttractor::UpdateParticles(float time, ParticleEmitter::ParticleList& particles)
{
    for (ParticleEmitter::ParticleList::iterator it = particles.begin(); it != particles.end(); ++it)
    {
        glm::vec3 diff = _Position - it->position;
        float r = glm::length(diff);
        it->position = it->position + (diff * (_Strength / (r*r)));
    }
}

ParticleRenderer::ParticleRenderer()
{
    Renderer::Instance()->SetHandler(TypeHash("particle"), this);
    
    Renderer::Instance()->GetEffectManager()->LoadEffect("/default/effects/particle.fx");
}

ParticleRenderer::~ParticleRenderer()
{
    Renderer::Instance()->GetEffectManager()->UnloadEffect("/default/effects/particle.fx");
}

void ParticleRenderer::Render(int count, Renderable** renderables, Viewport* viewport, EffectPass* effectPass)
{
    for (int i=0; i<count; i++)
    {
        ParticleRenderable& renderable = *static_cast<ParticleRenderable*>(renderables[i]);
        
        effectPass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", renderable.GetMVP());
        
        renderable._Emitter->Render(viewport, effectPass);
        
    }
}

#endif
