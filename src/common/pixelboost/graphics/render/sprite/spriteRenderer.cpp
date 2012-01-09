#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/indexBuffer.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/render/sprite/spriteRenderer.h"
#include "pixelboost/graphics/render/sprite/sprite.h"

namespace pixelboost
{

SpriteInstance::SpriteInstance(Sprite* sprite) :
	  sprite(sprite)
{
}
    
SpriteInstance::~SpriteInstance()
{
}

SpriteRenderer::SpriteRenderer(Renderer* renderer, int maxSpritesPerLayer, int numVertexBuffers)
    : IRenderer(renderer)
    , _CurrentVertexBuffer(0)
    , _MaxSprites(maxSpritesPerLayer)
{
    _IndexBuffer = pixelboost::GraphicsDevice::Instance()->CreateIndexBuffer(pixelboost::kBufferFormatStatic, _MaxSprites*6);
    
    for (int i=0; i<numVertexBuffers; i++)
    {
        VertexBuffer* vertexBuffer = pixelboost::GraphicsDevice::Instance()->CreateVertexBuffer(pixelboost::kBufferFormatDynamic, pixelboost::kVertexFormat_P_XYZ_UV, _MaxSprites*4);
        
        _VertexBuffers.push_back(vertexBuffer);
    }
    
    _IndexBuffer->Lock();
    unsigned short* indexBuffer = _IndexBuffer->GetData();
    for (int i=0; i<_MaxSprites; i++)
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

SpriteRenderer::~SpriteRenderer()
{
    for (VertexBufferList::iterator it = _VertexBuffers.begin(); it != _VertexBuffers.end(); ++it)
    {
        pixelboost::GraphicsDevice::Instance()->DestroyVertexBuffer(*it);
    }
    pixelboost::GraphicsDevice::Instance()->DestroyIndexBuffer(_IndexBuffer);
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
    
    VertexBuffer* vertexBuffer = 0;
    Vertex_PXYZ_UV* bufferData = 0;
    
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
        
        if (texture != it->sprite->_Sheet->_Texture)
        {
            RenderCurrentBuffer();
            texture = it->sprite->_Sheet->_Texture;
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
            bufferData = static_cast<Vertex_PXYZ_UV*>(vertexBuffer->GetData());
        }
        
        bufferData[0].position[2] = 0.f;
        bufferData[1].position[2] = 0.f;
        bufferData[2].position[2] = 0.f;
        bufferData[3].position[2] = 0.f;
        
        float cosRot = cos(it->rotation);
        float sinRot = sin(it->rotation);
        
        bufferData[0].position[0] = -it->scale[1] * cosRot + it->scale[0] * sinRot + it->position[0];
        bufferData[0].position[1] = it->scale[1] * sinRot + it->scale[0] * cosRot + it->position[1];
        bufferData[1].position[0] = it->scale[1] * cosRot + it->scale[0] * sinRot + it->position[0];
        bufferData[1].position[1] = -it->scale[1] * sinRot + it->scale[0] * cosRot + it->position[1];
        bufferData[2].position[0] = it->scale[1] * cosRot - it->scale[0] * sinRot + it->position[0];
        bufferData[2].position[1] = -it->scale[1] * sinRot - it->scale[0] * cosRot + it->position[1];
        bufferData[3].position[0] = -it->scale[1] * cosRot - it->scale[0] * sinRot + it->position[0];
        bufferData[3].position[1] = it->scale[1] * sinRot - it->scale[0] * cosRot + it->position[1];
        
        if (!it->sprite->_Rotated)
        {
            Vec2 min = it->sprite->_Position + Vec2(it->sprite->_Size[0] * it->crop[0], it->sprite->_Size[1] * it->crop[1]);
            Vec2 max = it->sprite->_Position + Vec2(it->sprite->_Size[0] * it->crop[2], it->sprite->_Size[1] * it->crop[3]);
            
            bufferData[0].uv[0] = min[0];
            bufferData[0].uv[1] = max[1],
            bufferData[1].uv[0] = min[0];
            bufferData[1].uv[1] = min[1];
            bufferData[2].uv[0] = max[0];
            bufferData[2].uv[1] = min[1];
            bufferData[3].uv[0] = max[0];
            bufferData[3].uv[1] = max[1];
        } else {
            Vec2 min = it->sprite->_Position + Vec2(it->sprite->_Size[1] * it->crop[3], it->sprite->_Size[0] * it->crop[2]);
            Vec2 max = it->sprite->_Position + Vec2(it->sprite->_Size[1] * it->crop[1], it->sprite->_Size[0] * it->crop[0]);
            
            bufferData[0].uv[0] = max[0];
            bufferData[0].uv[1] = max[1];
            bufferData[1].uv[0] = min[0];
            bufferData[1].uv[1] = max[1];
            bufferData[2].uv[0] = min[0];
            bufferData[2].uv[1] = min[1];
            bufferData[3].uv[0] = max[0];
            bufferData[3].uv[1] = min[1];
        }
        
        bufferData += 4;
        _SpritesRendered++;
    }
    
    RenderCurrentBuffer();
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    GraphicsDevice::Instance()->BindIndexBuffer(0);
    GraphicsDevice::Instance()->BindVertexBuffer(0);
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

bool SpriteRenderer::AddSpriteSheet(const std::string& name, std::shared_ptr<SpriteSheet> spriteSheet)
{
    SheetMap::iterator it = _SpriteSheets.find(name);
    
    if (it != _SpriteSheets.end())
        return true;
    
    _SpriteSheets[name] = spriteSheet;

    return true;
}
 
bool SpriteRenderer::RemoveSpriteSheet(std::shared_ptr<SpriteSheet> spriteSheet)
{
    for (SheetMap::iterator it = _SpriteSheets.begin(); it != _SpriteSheets.end(); ++it)
    {
        if (it->second == spriteSheet)
        {
            _SpriteSheets.erase(it);
            return true;
        }
    }
    
    return false;
}

bool SpriteRenderer::AttachToRenderer(RenderLayer* layer, const std::string& sheetName, const std::string& spriteName, Vec2 position, Vec3 rotation, Vec2 scale, BlendMode blendMode, Vec4 crop)
{
    Sprite* sprite = GetSprite(sheetName, spriteName);
    
    if (!sprite)
        return false;

    SpriteInstance instance(sprite);
    
    instance.crop = crop;
    instance.position = position - Vec2((0.5 - (crop[0]+crop[2])/2.f) * sprite->_Dimension[0], (0.5 - (crop[1]+crop[3])/2.f) * sprite->_Dimension[1]);
    instance.rotation = ((-rotation[2]-90.f) / 180.f) * M_PI;
    instance.scale = sprite->_Dimension * Vec2(crop[2]-crop[0], crop[3]-crop[1]) * scale * 0.5; // Scale now for later optimisation
    
    instance.blendMode = blendMode;
    
    _Instances[layer].push_back(instance);
    
    return true;
}
    
Sprite* SpriteRenderer::GetSprite(const std::string &sheetName, const std::string &spriteName) const
{
    std::shared_ptr<SpriteSheet> sheet = GetSpriteSheet(sheetName);
    
    if (!sheet)
        return 0;
    
    return sheet->GetSprite(spriteName);
}

std::shared_ptr<SpriteSheet> SpriteRenderer::GetSpriteSheet(const std::string& spriteSheet) const
{
    SheetMap::const_iterator it = _SpriteSheets.find(spriteSheet);
    
    if (it == _SpriteSheets.end())
        return std::shared_ptr<SpriteSheet>();
    
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
