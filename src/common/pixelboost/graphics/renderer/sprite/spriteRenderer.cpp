#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/assert.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/graphics/shader/manager.h"

using namespace pb;

SpriteRenderer* SpriteRenderer::_Instance = 0;

SpriteRenderable::SpriteRenderable(Uid entityId)
    : Renderable(entityId)
{
    _Sprite = 0;
    _Tint = glm::vec4(1,1,1,1);
}
    
SpriteRenderable::~SpriteRenderable()
{
}
    
Uid SpriteRenderable::GetType()
{
    return SpriteRenderable::GetStaticType();
}

Uid SpriteRenderable::GetStaticType()
{
    return TypeHash("pb::SpriteRenderable");
}

void SpriteRenderable::CalculateBounds()
{
    Sprite* sprite = GetSprite();
    if (sprite)
    {
        BoundingSphere bounds;
        glm::vec4 position = GetWorldMatrix() * glm::vec4(0,0,0,1);
        float scale = glm::length(GetWorldMatrix() * glm::vec4(0.5774,0.5774,0.5774,0));
        bounds.Set(glm::vec3(position.x, position.y, position.z), glm::max(sprite->_Size.x, sprite->_Size.y)*scale/2.f);
        SetBounds(bounds);
    }
}

void SpriteRenderable::CalculateWorldMatrix()
{
    if (_Sprite)
    {
        glm::mat4x4 worldMatrix = glm::translate(glm::mat4x4(), glm::vec3(-_Sprite->_Offset, 0));
        worldMatrix = glm::scale(worldMatrix, glm::vec3(_Sprite->_Size, 1));
        worldMatrix = _Transform * worldMatrix;
        SetWorldMatrix(worldMatrix);
    }
}
    
Shader* SpriteRenderable::GetShader()
{
    Shader* baseShader = Renderable::GetShader();
    if (baseShader)
        return baseShader;
    
    return Renderer::Instance()->GetShaderManager()->GetShader("/shaders/pb_texturedColor.shc");
}

Sprite* SpriteRenderable::GetSprite()
{
    return _Sprite;
}

void SpriteRenderable::SetSprite(Sprite* sprite)
{
    _Sprite = sprite;
    DirtyWorldMatrix();
}

void SpriteRenderable::SetTransform(const glm::mat4x4& transform)
{
    _Transform = transform;
    DirtyBounds();
    DirtyWorldMatrix();
}

void SpriteRenderable::SetTint(glm::vec4 tint)
{
    _Tint = tint;
}

SpriteRenderer::SpriteRenderer()
{
    PbAssert(!_Instance);
    
    _Instance = this;
    
    _MaxBatchSize = 100;
    _IndexBuffer = pb::GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, _MaxBatchSize * 6);
    _VertexBuffer = pb::GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P3_C4_UV, _MaxBatchSize * 4);
    
    _IndexBuffer->Lock();
    unsigned short* indicies = _IndexBuffer->GetData();
    for (int i=0; i<_MaxBatchSize; i++)
    {
        indicies[0] = (i*4) + 0;
        indicies[1] = (i*4) + 1;
        indicies[2] = (i*4) + 2;
        indicies[3] = (i*4) + 0;
        indicies[4] = (i*4) + 2;
        indicies[5] = (i*4) + 3;
        indicies += 6;
    }
    _IndexBuffer->Unlock();
    
    Renderer::Instance()->SetHandler(SpriteRenderable::GetStaticType(), this);
    
    Renderer::Instance()->GetShaderManager()->LoadShader("/shaders/pb_texturedColor.shc");
}

SpriteRenderer::~SpriteRenderer()
{
    _Instance = 0;
    
    Renderer::Instance()->GetShaderManager()->UnloadShader("/shaders/pb_texturedColor.shc");
    
    pb::GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
    pb::GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
}

SpriteRenderer* SpriteRenderer::Instance()
{
    return _Instance;
}
    
void SpriteRenderer::Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix)
{
    ShaderTechnique* technique = renderables[0]->GetShader()->GetTechnique(renderScheme);
    
    if (!technique)
        return;
    
    ShaderPass* shaderPass = technique->GetPass(0);
    shaderPass->Bind();
    shaderPass->GetShaderProgram()->SetUniform("PB_ProjectionMatrix", projectionMatrix);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetBlendMode(GraphicsDevice::kBlendOne, GraphicsDevice::kBlendOneMinusSourceAlpha);
    
    GraphicsDevice::Instance()->BindIndexBuffer(_IndexBuffer);
    
    Vertex_P3_C4_UV* bufferData = 0;
    
    Texture* texture = 0;
    
    _BatchSize = 0;
    
    shaderPass->GetShaderProgram()->SetUniform("PB_ModelViewMatrix", glm::mat4x4());
    shaderPass->GetShaderProgram()->SetUniform("_DiffuseTexture", 0);
    
    GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
    
    _VertexBuffer->Lock();
    
    for (int i=0; i<count; i++)
    {
        SpriteRenderable& renderable = *static_cast<SpriteRenderable*>(renderables[i]);
        
        Sprite* sprite = renderable._Sprite;
        if (!sprite)
            continue;
        
        if (texture != sprite->_Sheet->_Texture)
        {
            RenderBatch();
            texture = sprite->_Sheet->_Texture;
            
            GraphicsDevice::Instance()->BindTexture(texture);
        }
        
        if (_BatchSize == _MaxBatchSize)
        {
            RenderBatch();
        }
        
        bufferData = static_cast<Vertex_P3_C4_UV*>(_VertexBuffer->GetData()) + (_BatchSize * 4);
        
        if (!sprite->_Rotated)
        {
            glm::vec2 min = sprite->_UvPosition;
            glm::vec2 max = sprite->_UvPosition + glm::vec2(sprite->_UvSize[0], sprite->_UvSize[1]);
            
            bufferData[0].uv[0] = min[0];
            bufferData[0].uv[1] = max[1];
            bufferData[1].uv[0] = min[0];
            bufferData[1].uv[1] = min[1];
            bufferData[2].uv[0] = max[0];
            bufferData[2].uv[1] = min[1];
            bufferData[3].uv[0] = max[0];
            bufferData[3].uv[1] = max[1];
        } else {
            glm::vec2 min = sprite->_UvPosition + glm::vec2(sprite->_UvSize[1], sprite->_UvSize[0]);
            glm::vec2 max = sprite->_UvPosition;
            
            bufferData[0].uv[0] = max[0];
            bufferData[0].uv[1] = max[1];
            bufferData[1].uv[0] = min[0];
            bufferData[1].uv[1] = max[1];
            bufferData[2].uv[0] = min[0];
            bufferData[2].uv[1] = min[1];
            bufferData[3].uv[0] = max[0];
            bufferData[3].uv[1] = min[1];
        }
        
        glm::vec4 color = renderable._Tint;
        
        bufferData[0].color[0] = color.r;
        bufferData[0].color[1] = color.g;
        bufferData[0].color[2] = color.b;
        bufferData[0].color[3] = color.a;
        bufferData[1].color[0] = color.r;
        bufferData[1].color[1] = color.g;
        bufferData[1].color[2] = color.b;
        bufferData[1].color[3] = color.a;
        bufferData[2].color[0] = color.r;
        bufferData[2].color[1] = color.g;
        bufferData[2].color[2] = color.b;
        bufferData[2].color[3] = color.a;
        bufferData[3].color[0] = color.r;
        bufferData[3].color[1] = color.g;
        bufferData[3].color[2] = color.b;
        bufferData[3].color[3] = color.a;
        
        glm::vec4 a = renderable.GetModelViewMatrix() * glm::vec4(-0.5, -0.5, 0, 1);
        glm::vec4 b = renderable.GetModelViewMatrix() * glm::vec4(-0.5, 0.5, 0, 1);
        glm::vec4 c = renderable.GetModelViewMatrix() * glm::vec4(0.5, 0.5, 0, 1);
        glm::vec4 d = renderable.GetModelViewMatrix() * glm::vec4(0.5, -0.5, 0, 1);
                
        bufferData[0].position[0] = a.x;
        bufferData[0].position[1] = a.y;
        bufferData[0].position[2] = a.z;
        bufferData[1].position[0] = b.x;
        bufferData[1].position[1] = b.y;
        bufferData[1].position[2] = b.z;
        bufferData[2].position[0] = c.x;
        bufferData[2].position[1] = c.y;
        bufferData[2].position[2] = c.z;
        bufferData[3].position[0] = d.x;
        bufferData[3].position[1] = d.y;
        bufferData[3].position[2] = d.z;
        
        _BatchSize++;
    }
    
    RenderBatch();
    
    _VertexBuffer->Unlock(0);
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    GraphicsDevice::Instance()->BindTexture(0);
}

void SpriteRenderer::RenderBatch()
{
    if (_BatchSize != 0)
    {
        _VertexBuffer->Unlock(_BatchSize * 4);
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, _BatchSize * 6);
        _BatchSize = 0;
        _VertexBuffer->Lock();
    }
}
    
std::shared_ptr<SpriteSheet> SpriteRenderer::CreateSpriteSheet(const std::string &name)
{
    SheetMap::iterator it = _SpriteSheets.find(name);
    
    if (it != _SpriteSheets.end())
        return it->second;
    
    std::shared_ptr<SpriteSheet> sheet = SpriteSheet::Create();
    _SpriteSheets[name] = sheet;
    
    return sheet;
}

bool SpriteRenderer::LoadSpriteSheet(const std::string& name, const std::string& extension, bool createMips, bool hasPremultipliedAlpha)
{
    if (!CreateSpriteSheet(name))
        return false;
    
    std::shared_ptr<SpriteSheet> sheet = GetSpriteSheet(name);
    sheet->LoadSheet(name, extension, createMips);

    return true;
}
 
bool SpriteRenderer::UnloadSpriteSheet(const std::string& name)
{
    SheetMap::iterator sheetIt = _SpriteSheets.find(name);
    
    if (sheetIt != _SpriteSheets.end())
    {
        _SpriteSheets.erase(sheetIt);
        return true;
    }
    
    return false;
}
    
std::shared_ptr<SpriteSheet> SpriteRenderer::GetSpriteSheet(const std::string& sheetName) const
{
    SheetMap::const_iterator it = _SpriteSheets.find(sheetName);
    
    if (it == _SpriteSheets.end())
        return std::shared_ptr<SpriteSheet>();
    
    return it->second;
}
    
Sprite* SpriteRenderer::GetSprite(const std::string& spriteName) const
{
    SpriteMap::const_iterator it = _Sprites.find(spriteName);
    
    if (it == _Sprites.end())
        return 0;
    
    return it->second;
}
