#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/misc/pointer.h"

namespace pb
{

class IndexBuffer;
class SpriteSheet;
class Sprite;
class VertexBuffer;
    
enum BlendMode
{
    kBlendModeNormal,
    kBlendModeMultiply,
    kBlendModeScreen,
    kBlendModeAdd,
    kBlendModeOverlay,
    kBlendModeUnknown,
};

class SpriteInstance
{
public:
	SpriteInstance();
	~SpriteInstance();
    
public:
    glm::vec4 uv;
    glm::vec4 tint;
    glm::vec2 position;
    float rotation;
    glm::vec2 scale;
    BlendMode blendMode;
    
	Texture* texture;
    
    friend class SpriteRenderer;
};

class SpriteRenderer : public IRenderer
{
public:
	SpriteRenderer(int maxSpritesPerLayer=100, int numVertexBuffers=4);
	~SpriteRenderer();
    
    void Update(float time);
    void Render(RenderLayer* layer);
	
    std::shared_ptr<SpriteSheet> CreateSpriteSheet(const std::string& name);
    bool LoadSpriteSheet(const std::string& name, bool createMips=false);
    bool UnloadSpriteSheet(const std::string& name);
    
    bool AttachSprite(RenderLayer* layer, const std::string& spriteName, glm::vec2 position, glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f), glm::vec2 scale = glm::vec2(1.f, 1.f), BlendMode blendMode = kBlendModeNormal, glm::vec4 tint = glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec4 crop = glm::vec4(0.f, 0.f, 1.f, 1.f));
    bool AttachCustom(RenderLayer* layer, const std::string& sheetName, glm::vec2 position, glm::vec2 size, glm::vec4 uv, glm::vec3 rotation, BlendMode blendMode = kBlendModeNormal, glm::vec4 tint = glm::vec4(1.f, 1.f, 1.f, 1.f));
    
    std::shared_ptr<SpriteSheet> GetSpriteSheet(const std::string& sheetName) const;
    Sprite* GetSprite(const std::string& spriteName) const;
		
private:
    void SetBlendMode(BlendMode blendMode);
    void RenderCurrentBuffer();
    
	typedef std::vector<SpriteInstance> InstanceList;
    typedef std::map<RenderLayer*, InstanceList> InstanceListMap;
    
    typedef std::map<std::string, Sprite*> SpriteMap;
    typedef std::map<std::string, std::shared_ptr<SpriteSheet> > SheetMap;
	
	InstanceListMap _Instances;
    
    SpriteMap _Sprites;
	SheetMap _SpriteSheets;
    
private:
    typedef std::vector<VertexBuffer*> VertexBufferList;
    IndexBuffer* _IndexBuffer;
    VertexBufferList _VertexBuffers;
    
    int _SpritesRendered;
    int _CurrentVertexBuffer;
    int _MaxSprites;
    
    friend class SpriteSheet;
};
    
}
