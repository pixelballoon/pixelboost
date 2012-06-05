#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"
#include "pixelboost/misc/pointer.h"

namespace pb
{

class IndexBuffer;
class SpriteSheet;
class Sprite;
class VertexBuffer;

class SpriteRenderable : public Renderable
{
public:
    SpriteRenderable(Uid entityId);
    ~SpriteRenderable();
    
    virtual Uid GetRenderableType();
    
    virtual Effect* GetEffect();
    
    Sprite* Sprite;
    glm::mat4x4 Transform;
    glm::vec4 Tint;
    glm::vec4 Crop;
};

class SpriteRenderer : public IRenderer
{
public:
	SpriteRenderer();
	~SpriteRenderer();
    
    void Render(int count, Renderable* renderables, Viewport* viewport, EffectPass* effectPass);
	
    std::shared_ptr<SpriteSheet> CreateSpriteSheet(const std::string& name);
    bool LoadSpriteSheet(const std::string& name, bool createMips=false);
    bool UnloadSpriteSheet(const std::string& name);
    
    std::shared_ptr<SpriteSheet> GetSpriteSheet(const std::string& sheetName) const;
    Sprite* GetSprite(const std::string& spriteName) const;
		
private:
    typedef std::map<std::string, Sprite*> SpriteMap;
    typedef std::map<std::string, std::shared_ptr<SpriteSheet> > SheetMap;
	
    SpriteMap _Sprites;
	SheetMap _SpriteSheets;
    
private:
    IndexBuffer* _IndexBuffer;
    VertexBuffer* _VertexBuffer;
    
    int _SpritesRendered;
    
    friend class SpriteSheet;
};
    
}
