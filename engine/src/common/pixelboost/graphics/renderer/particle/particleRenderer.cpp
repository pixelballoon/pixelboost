#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/particle/particleSystem.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/particle/particleRenderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/resources/shaderResource.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/resource/resourceManager.h"

using namespace pb;

ParticleRenderer* ParticleRenderer::_Instance = 0;

ParticleRenderable::ParticleRenderable()
    : _System(0)
{
    
}

ParticleRenderable::~ParticleRenderable()
{
    
}

Uid ParticleRenderable::GetType()
{
    return ParticleRenderable::GetStaticType();
}

Uid ParticleRenderable::GetStaticType()
{
    return TypeHash("pb::ParticleRenderable");
}

void ParticleRenderable::CalculateBounds()
{
    SetBounds(BoundingSphere());
}

void ParticleRenderable::CalculateWorldMatrix()
{
    SetWorldMatrix(glm::mat4x4());
}

Shader* ParticleRenderable::GetShader()
{
    Shader* baseShader = Renderable::GetShader();
    if (baseShader)
        return baseShader;
    
    return ResourceManager::Instance()->GetPool("pixelboost")->GetResource<ShaderResource>("/shaders/pb_texturedColor.shc")->GetShader();
}

void ParticleRenderable::SetSystem(ParticleSystem* system)
{
    _System = system;

    if (_System)
    {
        _System->Transform = _Transform;
    }
}

ParticleSystem* ParticleRenderable::GetSystem()
{
    return _System;
}

void ParticleRenderable::SetTransform(const glm::mat4x4& transform)
{
    _Transform = transform;

    if (_System)
    {
        _System->Transform = transform;
    }
}

ParticleRenderer::ParticleRenderer()
{
    _Instance = this;
    
    _MaxParticles = 10000;
    
    _IndexBuffer = pb::GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, _MaxParticles*6);
    _VertexBuffer = pb::GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P3_C4_UV, _MaxParticles*4);
    
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
    
    Renderer::Instance()->SetHandler(ParticleRenderable::GetStaticType(), this);
}

ParticleRenderer::~ParticleRenderer()
{
    _Instance = 0;
    
    pb::GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
    pb::GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
}

ParticleRenderer* ParticleRenderer::Instance()
{
    return _Instance;
}

void ParticleRenderer::Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    Shader* shader = renderables[0]->GetShader();
    if (!shader)
        return;
    
    ShaderTechnique* technique = shader->GetTechnique(renderScheme);
    
    if (!technique)
        return;
    
    ShaderPass* shaderPass = technique->GetPass(0);
    shaderPass->Bind();
    shaderPass->GetShaderProgram()->SetUniform("PB_ProjectionMatrix", projectionMatrix);
    
    for (int i=0; i<count; i++)
    {
        ParticleRenderable& renderable = *static_cast<ParticleRenderable*>(renderables[i]);
        
        shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", renderable.GetModelViewMatrix());
        
        RenderSystem(renderable.GetSystem(), shaderPass);
    }
}

void ParticleRenderer::RenderSystem(ParticleSystem* system, ShaderPass* shaderPass)
{
    pb::Sprite* sprite = system->Definition->RenderSprite->SpriteDefinition;
    
    if (!sprite)
        return;
        
    _VertexBuffer->Lock();
    
    pb::Vertex_P3_C4_UV* vertexBuffer = static_cast<pb::Vertex_P3_C4_UV*>(_VertexBuffer->GetData());
    
    if (vertexBuffer)
    {
        int particleCount = 0;
        
        for (std::vector<Particle>::iterator it = system->Particles.begin(); it != system->Particles.end(); ++it, ++particleCount)
        {
            if (particleCount == _MaxParticles)
                break;
            
            glm::vec4 color = it->Color;
            float scale = it->Scale;
            glm::vec2 size = sprite->Size * glm::vec2(scale, scale);
            
            glm::mat4x4 transform = glm::translate(glm::mat4x4(), it->Position);
            transform = glm::scale(transform, glm::vec3(size, 1));
            transform = glm::rotate(transform, it->Rotation.x, glm::vec3(1,0,0));
            transform = glm::rotate(transform, it->Rotation.y, glm::vec3(0,1,0));
            transform = glm::rotate(transform, it->Rotation.z, glm::vec3(0,0,1));
            
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
            
            vertexBuffer[0].color[0] = color.r * color.a;
            vertexBuffer[0].color[1] = color.g * color.a;
            vertexBuffer[0].color[2] = color.b * color.a;
            vertexBuffer[0].color[3] = system->Definition->BlendMode == ParticleSystemDefinition::kBlendModeAdditive ? 0 : color.a;
            vertexBuffer[1].color[0] = color.r * color.a;
            vertexBuffer[1].color[1] = color.g * color.a;
            vertexBuffer[1].color[2] = color.b * color.a;
            vertexBuffer[1].color[3] = system->Definition->BlendMode == ParticleSystemDefinition::kBlendModeAdditive ? 0 : color.a;
            vertexBuffer[2].color[0] = color.r * color.a;
            vertexBuffer[2].color[1] = color.g * color.a;
            vertexBuffer[2].color[2] = color.b * color.a;
            vertexBuffer[2].color[3] = system->Definition->BlendMode == ParticleSystemDefinition::kBlendModeAdditive ? 0 : color.a;
            vertexBuffer[3].color[0] = color.r * color.a;
            vertexBuffer[3].color[1] = color.g * color.a;
            vertexBuffer[3].color[2] = color.b * color.a;
            vertexBuffer[3].color[3] = system->Definition->BlendMode == ParticleSystemDefinition::kBlendModeAdditive ? 0 : color.a;
            
            if (!sprite->Rotated)
            {
                glm::vec2 min = sprite->UvPosition;
                glm::vec2 max = sprite->UvPosition + sprite->UvSize;
                
                vertexBuffer[0].uv[0] = min[0];
                vertexBuffer[0].uv[1] = max[1],
                vertexBuffer[1].uv[0] = min[0];
                vertexBuffer[1].uv[1] = min[1];
                vertexBuffer[2].uv[0] = max[0];
                vertexBuffer[2].uv[1] = min[1];
                vertexBuffer[3].uv[0] = max[0];
                vertexBuffer[3].uv[1] = max[1];
            } else {
                glm::vec2 min = sprite->UvPosition + glm::vec2(sprite->UvSize[1], 0);
                glm::vec2 max = sprite->UvPosition + glm::vec2(0, sprite->UvSize[0]);
                
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
        
        GraphicsDevice::Instance()->BindTexture(0, system->Definition->RenderSprite->SpriteDefinition->_Texture);
        
        _VertexBuffer->Unlock(particleCount*4);
        
        GraphicsDevice::Instance()->BindIndexBuffer(_IndexBuffer);
        GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
        
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
        
        GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
        
        shaderPass->GetShaderProgram()->SetUniform("_DiffuseTexture", 0);
        
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, particleCount*6);
        
        GraphicsDevice::Instance()->BindIndexBuffer(0);
        GraphicsDevice::Instance()->BindVertexBuffer(0);
        
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
        GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
    }
    
    for (std::vector<ParticleSystem*>::iterator it = system->SubSystem.begin(); it != system->SubSystem.end(); ++it)
    {
        RenderSystem(*it, shaderPass);
    }
}
