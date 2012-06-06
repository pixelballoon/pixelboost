#ifndef PIXELBOOST_DISABLE_GRAPHICS

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

Effect* ParticleRenderable::GetEffect()
{
    Effect* baseEffect = Renderable::GetEffect();
    if (baseEffect)
        return baseEffect;
    
    return Renderer::Instance()->GetEffectManager()->GetEffect("/default/effects/sprite.fx");
}

ParticleEmitter* ParticleRenderable::GetEmitter()
{
    return _Emitter;
}

ParticleEmitter::Particle::Particle(ParticleEmitter::Config* config)
    : emitterConfig(config)
{
    emitterConfig->refCount++;
}

ParticleEmitter::Particle::Particle(const Particle& rhs)
{
    Assign(rhs);
}

ParticleEmitter::Particle::~Particle()
{
    emitterConfig->refCount--;
    
    if (emitterConfig->refCount == 0)
    {
        delete emitterConfig;
    }
}

ParticleEmitter::Particle& ParticleEmitter::Particle::operator=(const Particle& rhs)
{
    Assign(rhs);
    
    return *this;
}

void ParticleEmitter::Particle::Assign(const Particle& rhs)
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
    refCount = 1;
    
    particlesPerUpdate = 1.f;
    
    initialScale = 1.f;
    life = 1.f;
    
    startScale = glm::vec2(1.f, 1.f);
    endScale = glm::vec2(1.f, 1.f);
    startColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    endColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    minPosOffset = glm::vec2(0.f, 0.f);
    maxPosOffset = glm::vec2(0.f, 0.f);
    minRotOffset = glm::vec3(0.f, 0.f, 0.f);
    maxRotOffset = glm::vec3(0.f, 0.f, 0.f);
    minRotVelocity = glm::vec3(0.f, 0.f, 0.f);
    maxRotVelocity = glm::vec3(0.f, 0.f, 0.f);
    minPosVelocity = glm::vec2(0.f, 0.f);
    maxPosVelocity = glm::vec2(0.f, 0.f);
    gravity = glm::vec2(0.f, 0.f);
}

ParticleEmitter::Config::~Config()
{

}

ParticleEmitter::ParticleEmitter(int maxParticles)
    : _EmitCount(0)
    , _MaxParticles(maxParticles)
{
    _Config = new Config();
    
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
    for (ModifierList::iterator it = _Modifiers.begin(); it != _Modifiers.end(); ++it)
    {
        delete *it;
    }
    
    pb::GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    pb::GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
    
    _Config->refCount--;
    
    if (_Config->refCount == 0)
        delete _Config;
}

void ParticleEmitter::Update(float time)
{
    if (!_Config->sprites.size())
        return;
    
    for (ModifierList::iterator it = _Modifiers.begin(); it != _Modifiers.end(); ++it)
    {
        (*it)->Update(time);
    }
    
    _EmitCount += _Config->particlesPerUpdate;
    
    while (_EmitCount >= 1.f)
    {
        if (_Particles.size() < _MaxParticles)
        {
            Particle particle(_Config);
            particle.position[0] = _Position[0] + _Config->minPosOffset[0] + (_Config->maxPosOffset[0]-_Config->minPosOffset[0]) * (float)rand()/(float)RAND_MAX;
            particle.position[1] = _Position[1] + _Config->minPosOffset[1] + (_Config->maxPosOffset[1]-_Config->minPosOffset[1]) * (float)rand()/(float)RAND_MAX;
            particle.rotation = _Config->minRotOffset + (_Config->maxRotOffset-_Config->minRotOffset) * (float)rand()/(float)RAND_MAX;
            particle.rotationVelocity = _Config->minRotVelocity + (_Config->maxRotVelocity-_Config->minRotVelocity) * (float)rand()/(float)RAND_MAX;
            particle.positionVelocity[0] = _Config->minPosVelocity[0] + (_Config->maxPosVelocity[0]-_Config->minPosVelocity[0]) * (float)rand()/(float)RAND_MAX;
            particle.positionVelocity[1] = _Config->minPosVelocity[1] + (_Config->maxPosVelocity[1]-_Config->minPosVelocity[1]) * (float)rand()/(float)RAND_MAX;
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
    if (!_Particles.size())
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
            glm::vec2 size = sprite->_Dimension * scale;
            
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
                glm::vec2 min = sprite->_Position;
                glm::vec2 max = sprite->_Position + sprite->_Size;
                
                vertexBuffer[0].uv[0] = min[0];
                vertexBuffer[0].uv[1] = max[1],
                vertexBuffer[1].uv[0] = min[0];
                vertexBuffer[1].uv[1] = min[1];
                vertexBuffer[2].uv[0] = max[0];
                vertexBuffer[2].uv[1] = min[1];
                vertexBuffer[3].uv[0] = max[0];
                vertexBuffer[3].uv[1] = max[1];
            } else {
                glm::vec2 min = sprite->_Position + glm::vec2(sprite->_Size[1], 0);
                glm::vec2 max = sprite->_Position + glm::vec2(0, sprite->_Size[0]);
                
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
        
        glm::mat4x4 viewProjectionMatrix = viewport->GetCamera()->ProjectionMatrix * viewport->GetCamera()->ViewMatrix;
        
        effectPass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix);
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

bool ParticleEmitter::Load(const std::string& file)
{
    return false;
}

void ParticleEmitter::LoadSpriteSheet(const std::string& file, bool createMips)
{
    _Config->spriteSheet = SpriteSheet::Create();
    _Config->spriteSheet->LoadSheet(file, createMips);
}

void ParticleEmitter::SetSpriteSheet(std::shared_ptr<SpriteSheet> spriteSheet)
{
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

ParticleEmitter::Config& ParticleEmitter::GetConfig()
{
    return *_Config;
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

ParticleAttractor::ParticleAttractor(ParticleEmitter* emitter, const glm::vec2& position, float strength)
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
        glm::vec2 diff = _Position - it->position;
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
        
        renderable._Emitter->Render(viewport, effectPass);
        
    }
}

#endif
