#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/render/sprite/spriteRenderer.h"
#include "libpixel/graphics/render/sprite/sprite.h"

namespace libpixel
{

SpriteInstance::SpriteInstance(Sprite* sprite) :
	  _Sprite(sprite)
{
	_Sprite->_Sheet->_RefCount++;
}
    
SpriteInstance::~SpriteInstance()
{
    _Sprite->_Sheet->_RefCount--;
}

SpriteRenderer::SpriteRenderer()
{
    GLfloat glVertices[8] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f };
    
    glGenBuffers(1, &_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer);
	
    Vertex_PXYZ_UV* vertexBuffer = _Vertices;

    for (int i=0; i<4; i++)
    {
        vertexBuffer[i].position[0] = glVertices[i*2];
        vertexBuffer[i].position[1] = glVertices[(i*2)+1];
        vertexBuffer[i].position[2] = 0.f;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    GLushort indexBuffer[6] = {0, 1, 2, 0, 2, 3};
    glGenBuffers(1, &_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indexBuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

SpriteRenderer::~SpriteRenderer()
{
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
    
    glBindBuffer(GL_ARRAY_BUFFER, _VertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IndexBuffer);
    
    GLsizei stride = sizeof(Vertex_PXYZ_UV);
    glVertexPointer(3, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_UV, position));
    glTexCoordPointer(2, GL_FLOAT, stride, (void*)offsetof(Vertex_PXYZ_UV, uv));
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    Vertex_PXYZ_UV* vertexBuffer = _Vertices;
    
    for (InstanceList::iterator it = instanceList.begin(); it != instanceList.end(); ++it)
    {
        switch (it->_BlendMode)
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
                #ifdef LIBPIXEL_GRAPHICS_PREMULTIPLIED_ALPHA
                    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                #else
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                #endif
                break;
        }
                
        if (!it->_Sprite->_Rotated)
        {
            Vec2 min = it->_Sprite->_Position;
            Vec2 max = it->_Sprite->_Position + it->_Sprite->_Size;
            
            vertexBuffer[0].uv[0] = min[0];
            vertexBuffer[0].uv[1] = max[1],
            vertexBuffer[1].uv[0] = min[0];
            vertexBuffer[1].uv[1] = min[1];
            vertexBuffer[2].uv[0] = max[0];
            vertexBuffer[2].uv[1] = min[1];
            vertexBuffer[3].uv[0] = max[0];
            vertexBuffer[3].uv[1] = max[1];
        } else {
            Vec2 min = it->_Sprite->_Position + Vec2(it->_Sprite->_Size[1], 0);
            Vec2 max = it->_Sprite->_Position + Vec2(0, it->_Sprite->_Size[0]);

            vertexBuffer[0].uv[0] = max[0];
            vertexBuffer[0].uv[1] = min[1];
            vertexBuffer[1].uv[0] = min[0];
            vertexBuffer[1].uv[1] = min[1];
            vertexBuffer[2].uv[0] = min[0];
            vertexBuffer[2].uv[1] = max[1];
            vertexBuffer[3].uv[0] = max[0];
            vertexBuffer[3].uv[1] = max[1];
        }
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex_PXYZ_UV) * 4, _Vertices, GL_DYNAMIC_DRAW);
        it->_Sprite->_Sheet->_Texture->Bind(0);
        
        glPushMatrix();
        
        glTranslatef(it->_Position[0], it->_Position[1], 0.f);
        glScalef(it->_Scale[0], it->_Scale[1], 1.f);        
        glTranslatef(it->_Offset[0], it->_Offset[1], 0.f);
        glRotatef(it->_Rotation[0], 1, 0, 0);
        glRotatef(it->_Rotation[1], 0, 1, 0);
        glRotatef(it->_Rotation[2], 0, 0, 1);
        
        glScalef(it->_Sprite->_Dimension[0], it->_Sprite->_Dimension[1], 1.f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
        
        glPopMatrix();
    }
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

bool SpriteRenderer::AddSpriteSheet(const std::string& name, SpriteSheet* spriteSheet, bool takeOwnership)
{
    SheetMap::iterator it = _SpriteSheets.find(name);
    
    if (it != _SpriteSheets.end())
        return true;
    
    SpriteRendererSheet sheet;
    sheet.spriteSheet = spriteSheet;
    sheet.hasOwnership = takeOwnership;
    _SpriteSheets[name] = sheet;

    return true;
}
 
bool SpriteRenderer::RemoveSpriteSheet(libpixel::SpriteSheet *spriteSheet)
{
    for (SheetMap::iterator it = _SpriteSheets.begin(); it != _SpriteSheets.end(); ++it)
    {
        if (it->second.spriteSheet == spriteSheet)
        {
            if (it->second.hasOwnership)
                delete it->second.spriteSheet;
            
            _SpriteSheets.erase(it);
            return true;
        }
    }
    
    return false;
}

bool SpriteRenderer::AttachToRenderer(RenderLayer* layer, const std::string& sheetName, const std::string& spriteName, Vec2 position, Vec3 rotation, Vec2 scale, BlendMode blendMode, Vec2 offset)
{
    Sprite* sprite = GetSprite(sheetName, spriteName);
    
    if (!sprite)
        return false;

    SpriteInstance instance(sprite);
    
    instance._Position = position;
    instance._Rotation = rotation;
    instance._Scale = scale;
    instance._Offset = offset;
    instance._BlendMode = blendMode;
    
    _Instances[layer].push_back(instance);
    
    return true;
}
    
Sprite* SpriteRenderer::GetSprite(const std::string &sheetName, const std::string &spriteName) const
{
    SpriteSheet* sheet = GetSpriteSheet(sheetName);
    
    if (!sheet)
        return 0;
    
    return sheet->GetSprite(spriteName);
}

SpriteSheet* SpriteRenderer::GetSpriteSheet(const std::string& spriteSheet) const
{
    SheetMap::const_iterator it = _SpriteSheets.find(spriteSheet);
    
    if (it == _SpriteSheets.end())
        return 0;
    
    return it->second.spriteSheet;
}

}
