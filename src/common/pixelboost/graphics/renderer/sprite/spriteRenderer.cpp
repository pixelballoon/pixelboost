#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/effect/effect.h"
#include "pixelboost/graphics/effect/manager.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"

namespace pb
{

SpriteRenderable::SpriteRenderable(Uid entityId)
    : Renderable(entityId)
{
    Crop = glm::vec4(0,0,1,1);
    Tint = glm::vec4(1,1,1,1);
}
    
SpriteRenderable::~SpriteRenderable()
{
}
    
Uid SpriteRenderable::GetRenderableType()
{
    return TypeHash("sprite");
}
    
Effect* SpriteRenderable::GetEffect()
{
    Effect* baseEffect = Renderable::GetEffect();
    if (baseEffect)
        return baseEffect;
    
    return Renderer::Instance()->GetEffectManager()->GetEffect("/default/effects/sprite.fx");
}

SpriteRenderer::SpriteRenderer()
{
    _IndexBuffer = pb::GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, 6);
    _VertexBuffer = pb::GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P_XYZ_UV, 4);
    _VertexBuffer->Lock();
    Vertex_PXYZ_UV* vertices = static_cast<Vertex_PXYZ_UV*>(_VertexBuffer->GetData());
    vertices[0].position[0] = -0.5;
    vertices[0].position[1] = -0.5;
    vertices[0].position[2] = 0;
    vertices[1].position[0] = -0.5;
    vertices[1].position[1] = 0.5;
    vertices[1].position[2] = 0;
    vertices[2].position[0] = 0.5;
    vertices[2].position[1] = 0.5;
    vertices[2].position[2] = 0;
    vertices[3].position[0] = 0.5;
    vertices[3].position[1] = -0.5;
    vertices[3].position[2] = 0;
    _VertexBuffer->Unlock();
    
    _IndexBuffer->Lock();
    unsigned short* indicies = _IndexBuffer->GetData();
    indicies[0] = 0;
    indicies[1] = 1;
    indicies[2] = 2;
    indicies[3] = 0;
    indicies[4] = 2;
    indicies[5] = 3;
    _IndexBuffer->Unlock();
    
    Renderer::Instance()->SetHandler(TypeHash("sprite"), this);
    
    Renderer::Instance()->GetEffectManager()->LoadEffect("/default/effects/sprite.fx");
}

SpriteRenderer::~SpriteRenderer()
{
    Renderer::Instance()->GetEffectManager()->UnloadEffect("/default/effects/sprite.fx");
    
    pb::GraphicsDevice::Instance()->DestroyVertexBuffer(_VertexBuffer);
    pb::GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
}
    
void SpriteRenderer::Render(int count, Renderable* renderables, Viewport* viewport, EffectPass* effectPass)
{
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, true);
    
    GraphicsDevice::Instance()->BindIndexBuffer(_IndexBuffer);
    
    Vertex_PXYZ_UV* bufferData = 0;
    
    Texture* texture = 0;
    
    _SpritesRendered = 0;
    
    for (int i=0; i<count; i++)
    {
        SpriteRenderable& renderable = static_cast<SpriteRenderable&>(renderables[i]);
        
        glm::vec4 uv(0,0,1,1);
        
        Sprite* sprite = renderable.Sprite;
        if (!sprite)
            continue;
        
        texture = sprite->_Sheet->_Texture;
        
        if (!sprite->_Rotated)
        {
            glm::vec2 min = sprite->_Position + glm::vec2(sprite->_Size[0] * renderable.Crop[0], sprite->_Size[1] * renderable.Crop[1]);
            glm::vec2 max = sprite->_Position + glm::vec2(sprite->_Size[0] * renderable.Crop[2], sprite->_Size[1] * renderable.Crop[3]);
            
            uv[0] = min[0];
            uv[1] = min[1];
            uv[2] = max[0];
            uv[3] = max[1];
        } else {
            glm::vec2 min = sprite->_Position + glm::vec2(sprite->_Size[1] * renderable.Crop[3], sprite->_Size[0] * renderable.Crop[2]);
            glm::vec2 max = sprite->_Position + glm::vec2(sprite->_Size[1] * renderable.Crop[1], sprite->_Size[0] * renderable.Crop[0]);
            
            uv[0] = min[0];
            uv[1] = min[1];
            uv[2] = max[0];
            uv[3] = max[1];
        }
        
        GraphicsDevice::Instance()->BindTexture(texture);
        
        _VertexBuffer->Lock();
        bufferData = static_cast<Vertex_PXYZ_UV*>(_VertexBuffer->GetData());
        
        glm::mat4x4 viewProjectionMatrix = viewport->GetCamera()->ProjectionMatrix * viewport->GetCamera()->ViewMatrix;
        viewProjectionMatrix = renderable.Transform * viewProjectionMatrix;
        viewProjectionMatrix = glm::scale(viewProjectionMatrix, glm::vec3(sprite->_Dimension, 1));
        effectPass->GetShaderProgram()->SetUniform("modelViewProjectionMatrix", viewProjectionMatrix);
        effectPass->GetShaderProgram()->SetUniform("diffuseColor", renderable.Tint);
        effectPass->GetShaderProgram()->SetUniform("diffuseTexture", 0);
        
        bufferData[0].uv[0] = uv[0];
        bufferData[0].uv[1] = uv[3];
        bufferData[1].uv[0] = uv[0];
        bufferData[1].uv[1] = uv[1];
        bufferData[2].uv[0] = uv[2];
        bufferData[2].uv[1] = uv[1];
        bufferData[3].uv[0] = uv[2];
        bufferData[3].uv[1] = uv[3];
        
        _VertexBuffer->Unlock();
        
        GraphicsDevice::Instance()->BindVertexBuffer(_VertexBuffer);
        GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, 6);
    }
    
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateDepthTest, true);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateBlend, false);
    GraphicsDevice::Instance()->SetState(GraphicsDevice::kStateTexture2D, false);
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    GraphicsDevice::Instance()->BindTexture(0);
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

bool SpriteRenderer::LoadSpriteSheet(const std::string& name, bool createMips)
{
    if (!CreateSpriteSheet(name))
        return false;
    
    std::shared_ptr<SpriteSheet> sheet = GetSpriteSheet(name);
    sheet->LoadSheet(name, createMips);

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

}

#endif
