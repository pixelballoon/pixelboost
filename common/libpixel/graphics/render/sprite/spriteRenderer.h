#pragma once

#include <map>
#include <string>
#include <vector>

#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/device/vertexBuffer.h"
#include "libpixel/graphics/render/common/irenderer.h"
#include "libpixel/math/maths.h"

namespace libpixel
{

class SpriteSheet;
class Sprite;
    
enum BlendMode
{
    kBlendModeNormal,
    kBlendModeMultiply,
    kBlendModeScreen,
    kBlendModeAdd,
    kBlendModeOverlay,
};

class SpriteInstance
{
public:
	SpriteInstance(Sprite* sprite);
	~SpriteInstance();
    
public:
    Vec4 crop;
    Vec2 position;
    Vec3 rotation;
    Vec2 scale;
    BlendMode blendMode;
    
	Sprite* sprite;
    
    friend class SpriteRenderer;
};

class SpriteRenderer : public IRenderer
{
public:
	SpriteRenderer(int maxSpritesPerLayer=100);
	~SpriteRenderer();
    
    void Update(float time);
    void Render(RenderLayer* layer);
	
    bool AddSpriteSheet(const std::string& name, SpriteSheet* spriteSheet, bool takeOwnership);
    bool RemoveSpriteSheet(SpriteSheet* spriteSheet);
    
    bool AttachToRenderer(RenderLayer* layer, const std::string& sheetName, const std::string& spriteName, Vec2 position, Vec3 rotation = Vec3(0.f, 0.f, 0.f), Vec2 scale = Vec2(1.f, 1.f), BlendMode blendMode = kBlendModeNormal, Vec4 crop = Vec4(0.f, 0.f, 1.f, 1.f));
    
    Sprite* GetSprite(const std::string& sheetName, const std::string& spriteName) const;
		
private:
    SpriteSheet* GetSpriteSheet(const std::string& spriteSheet) const;
    
	typedef std::vector<SpriteInstance> InstanceList;
    typedef std::map<RenderLayer*, InstanceList> InstanceListMap;
    
    struct SpriteRendererSheet
    {
        SpriteSheet* spriteSheet;
        bool hasOwnership;
    };
    
    typedef std::map<std::string, SpriteRendererSheet> SheetMap;
	
	InstanceListMap _Instances;
    
	SheetMap _SpriteSheets;
    
private:
    IndexBuffer* _IndexBuffer;
    VertexBuffer* _VertexBuffer;
    
    int _MaxSprites;
};
    
}
