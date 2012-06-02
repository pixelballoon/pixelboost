#ifndef PIXELBOOST_DISABLE_GRAPHICS

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"

namespace pb
{

SpriteInstance::SpriteInstance()
{
}
    
SpriteInstance::~SpriteInstance()
{
}

SpriteRenderer::SpriteRenderer(int maxSpritesPerLayer, int numVertexBuffers)
    : _CurrentVertexBuffer(0)
    , _MaxSprites(maxSpritesPerLayer)
{
    _IndexBuffer = pb::GraphicsDevice::Instance()->CreateIndexBuffer(pb::kBufferFormatStatic, _MaxSprites*6);
    
    for (int i=0; i<numVertexBuffers; i++)
    {
        VertexBuffer* vertexBuffer = pb::GraphicsDevice::Instance()->CreateVertexBuffer(pb::kBufferFormatDynamic, pb::kVertexFormat_P_XYZ_RGBA_UV, _MaxSprites*4);
        
        _VertexBuffers.push_back(vertexBuffer);
    }
    
    _IndexBuffer->Lock();
    unsigned short* indexBuffer = _IndexBuffer->GetData();
    for (int i=0; i<_MaxSprites; i++)
    {
        indexBuffer[0] = (i*4) + 2;
        indexBuffer[1] = (i*4) + 1;
        indexBuffer[2] = (i*4) + 0;
        indexBuffer[3] = (i*4) + 3;
        indexBuffer[4] = (i*4) + 2;
        indexBuffer[5] = (i*4) + 0;
        
        indexBuffer += 6;
    }
    _IndexBuffer->Unlock();
}

SpriteRenderer::~SpriteRenderer()
{
    for (VertexBufferList::iterator it = _VertexBuffers.begin(); it != _VertexBuffers.end(); ++it)
    {
        pb::GraphicsDevice::Instance()->DestroyVertexBuffer(*it);
    }
    pb::GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
}
    
void SpriteRenderer::Update(float time)
{
    _Instances.clear();
}
    
void SpriteRenderer::Render(RenderLayer* layer)
{
    InstanceList& instanceList = _Instances[layer];
    if (!instanceList.size())
        return;
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    
    _IndexBuffer->Bind();
        
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    VertexBuffer* vertexBuffer = 0;
    Vertex_PXYZ_RGBA_UV* bufferData = 0;
    
    BlendMode blendMode = kBlendModeUnknown;
    Texture* texture = 0;
    int currentVertexBuffer = -1;
    
    _SpritesRendered = 0;
    
    for (InstanceList::iterator it = instanceList.begin(); it != instanceList.end(); ++it)
    {
        if (it->blendMode != blendMode)
        {
            RenderCurrentBuffer();
            blendMode = it->blendMode;
            SetBlendMode(blendMode);
        }
        
        if (texture != it->texture)
        {
            RenderCurrentBuffer();
            texture = it->texture;
            texture->Bind();
        }
        
        if (_SpritesRendered == _MaxSprites)
        {
            RenderCurrentBuffer();
        }
        
        if (_CurrentVertexBuffer != currentVertexBuffer)
        {
            currentVertexBuffer = _CurrentVertexBuffer;
            vertexBuffer = _VertexBuffers[_CurrentVertexBuffer];
            vertexBuffer->Lock();
            bufferData = static_cast<Vertex_PXYZ_RGBA_UV*>(vertexBuffer->GetData());
        }
        
        bufferData[0].color[0] = it->tint[0];
        bufferData[0].color[1] = it->tint[1];
        bufferData[0].color[2] = it->tint[2];
        bufferData[0].color[3] = it->tint[3];
        bufferData[1].color[0] = it->tint[0];
        bufferData[1].color[1] = it->tint[1];
        bufferData[1].color[2] = it->tint[2];
        bufferData[1].color[3] = it->tint[3];
        bufferData[2].color[0] = it->tint[0];
        bufferData[2].color[1] = it->tint[1];
        bufferData[2].color[2] = it->tint[2];
        bufferData[2].color[3] = it->tint[3];
        bufferData[3].color[0] = it->tint[0];
        bufferData[3].color[1] = it->tint[1];
        bufferData[3].color[2] = it->tint[2];
        bufferData[3].color[3] = it->tint[3];
        
        float cosRot = cos(it->rotation);
        float sinRot = sin(it->rotation);
        
        bufferData[0].position[0] = -it->scale[1] * cosRot + it->scale[0] * sinRot + it->position[0];
        bufferData[0].position[1] = it->scale[1] * sinRot + it->scale[0] * cosRot + it->position[1];
        bufferData[0].position[2] = 0.f;
        bufferData[1].position[0] = it->scale[1] * cosRot + it->scale[0] * sinRot + it->position[0];
        bufferData[1].position[1] = -it->scale[1] * sinRot + it->scale[0] * cosRot + it->position[1];
        bufferData[1].position[2] = 0.f;
        bufferData[2].position[0] = it->scale[1] * cosRot - it->scale[0] * sinRot + it->position[0];
        bufferData[2].position[1] = -it->scale[1] * sinRot - it->scale[0] * cosRot + it->position[1];
        bufferData[2].position[2] = 0.f;
        bufferData[3].position[0] = -it->scale[1] * cosRot - it->scale[0] * sinRot + it->position[0];
        bufferData[3].position[1] = it->scale[1] * sinRot - it->scale[0] * cosRot + it->position[1];
        bufferData[3].position[2] = 0.f;
        
        bufferData[0].uv[0] = it->uv[0];
        bufferData[0].uv[1] = it->uv[3];
        bufferData[1].uv[0] = it->uv[0];
        bufferData[1].uv[1] = it->uv[1];
        bufferData[2].uv[0] = it->uv[2];
        bufferData[2].uv[1] = it->uv[1];
        bufferData[3].uv[0] = it->uv[2];
        bufferData[3].uv[1] = it->uv[3];
        
        bufferData += 4;
        _SpritesRendered++;
    }
    
    RenderCurrentBuffer();
    
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
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
        for (InstanceListMap::iterator layerIt = _Instances.begin(); layerIt != _Instances.end(); ++layerIt)
        {
            for (InstanceList::iterator instanceIt = layerIt->second.begin(); instanceIt != layerIt->second.end();)
            {
                if (instanceIt->texture == sheetIt->second->_Texture)
                {
                    instanceIt = layerIt->second.erase(instanceIt);
                } else {
                    instanceIt++;
                }
            }
        }
        
        _SpriteSheets.erase(sheetIt);
        return true;
    }
    
    return false;
}

bool SpriteRenderer::AttachSprite(RenderLayer* layer, const std::string& spriteName, glm::vec2 position, glm::vec3 rotation, glm::vec2 scale, BlendMode blendMode, glm::vec4 tint, glm::vec4 crop)
{
    Sprite* sprite = GetSprite(spriteName);
    
    if (!sprite)
        return false;
    
#ifdef PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
    float alpha = color[3];
    color = color*alpha;
    color[3] = alpha;
#endif
    
    glm::vec2 dimension = sprite->_Dimension;

    SpriteInstance instance;
    
    if (!sprite->_Rotated)
    {
        glm::vec2 min = sprite->_Position + glm::vec2(sprite->_Size[0] * crop[0], sprite->_Size[1] * crop[1]);
        glm::vec2 max = sprite->_Position + glm::vec2(sprite->_Size[0] * crop[2], sprite->_Size[1] * crop[3]);
        
        instance.uv[0] = min[0];
        instance.uv[1] = min[1];
        instance.uv[2] = max[0];
        instance.uv[3] = max[1];
    } else {
        glm::vec2 min = sprite->_Position + glm::vec2(sprite->_Size[1] * crop[3], sprite->_Size[0] * crop[2]);
        glm::vec2 max = sprite->_Position + glm::vec2(sprite->_Size[1] * crop[1], sprite->_Size[0] * crop[0]);
        
        instance.uv[0] = min[0];
        instance.uv[1] = min[1];
        instance.uv[2] = max[0];
        instance.uv[3] = max[1];
    }
    
    if (sprite->_Rotated)
    {
        rotation[2] -= 90.f;
        float temp = dimension.x;
        dimension.x = dimension.y;
        dimension.y = temp;
    }
    
    instance.position = position - glm::vec2((0.5 - (crop[0]+crop[2])/2.f) * dimension[0], (0.5 - (crop[1]+crop[3])/2.f) * dimension[1]);
    instance.rotation = ((-rotation[2]-90.f) / 180.f) * M_PI;
    instance.scale = dimension * glm::vec2(crop[2]-crop[0], crop[3]-crop[1]) * scale * 0.5f;
    instance.blendMode = blendMode;
    instance.tint = tint;
    instance.texture = sprite->_Sheet->_Texture;
    
    _Instances[layer].push_back(instance);
    
    return true;
}
    
bool SpriteRenderer::AttachCustom(RenderLayer* layer, const std::string& sheetName, glm::vec2 position, glm::vec2 size, glm::vec4 uv, glm::vec3 rotation, BlendMode blendMode, glm::vec4 tint)
{
    std::shared_ptr<SpriteSheet> sheet = GetSpriteSheet(sheetName);
    
    if (!sheet)
        return false;
    
    SpriteInstance instance;
    
    instance.uv = uv;
    instance.position = position - size*0.5f;
    instance.rotation = ((-rotation[2]-90.f) / 180.f) * M_PI;
    instance.scale = size * 0.5f;
    instance.blendMode = blendMode;
    instance.tint = tint;
    instance.texture = sheet->_Texture;
    
    _Instances[layer].push_back(instance);
    
    return true;
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
    
void SpriteRenderer::SetBlendMode(BlendMode blendMode)
{
    switch (blendMode)
    {
        case kBlendModeMultiply:
            glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case kBlendModeScreen:
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
            break;
        case kBlendModeAdd:
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case kBlendModeOverlay:
            glBlendFunc(GL_DST_COLOR, GL_ONE);
            break;
        case kBlendModeNormal:
#ifdef PIXELBOOST_GRAPHICS_PREMULTIPLIED_ALPHA
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#else
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
            break;
        case kBlendModeUnknown:
            break;
    }
}
    
void SpriteRenderer::RenderCurrentBuffer()
{
    if (_SpritesRendered == 0)
        return;
    
    _VertexBuffers[_CurrentVertexBuffer]->Unlock(_SpritesRendered*4);
    _VertexBuffers[_CurrentVertexBuffer]->Bind();
    
    GraphicsDevice::Instance()->DrawElements(GraphicsDevice::kElementTriangles, _SpritesRendered*6);
    
    _CurrentVertexBuffer++;
    
    if (_CurrentVertexBuffer >= _VertexBuffers.size())
        _CurrentVertexBuffer = 0;

    _SpritesRendered = 0;
}

}

#endif
