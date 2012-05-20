#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/device/vertexBuffer.h"
#include "pixelboost/graphics/render/common/irenderer.h"
#include "pixelboost/math/maths.h"
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
    Vec4 uv;
    Vec4 tint;
    Vec2 position;
    float rotation;
    Vec2 scale;
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
    
    bool AttachSprite(RenderLayer* layer, const std::string& spriteName, Vec2 position, Vec3 rotation = Vec3(0.f, 0.f, 0.f), Vec2 scale = Vec2(1.f, 1.f), BlendMode blendMode = kBlendModeNormal, Vec4 tint = Vec4(1.f, 1.f, 1.f, 1.f), Vec4 crop = Vec4(0.f, 0.f, 1.f, 1.f));
    bool AttachCustom(RenderLayer* layer, const std::string& sheetName, Vec2 position, Vec2 size, Vec4 uv, Vec3 rotation, BlendMode blendMode = kBlendModeNormal, Vec4 tint = Vec4(1.f, 1.f, 1.f, 1.f));
    
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
