#ifndef LIBPIXEL__COMMON__GRAPHICS__RENDER__SPRITE__SPRITERENDERER__H
#define LIBPIXEL__COMMON__GRAPHICS__RENDER__SPRITE__SPRITERENDERER__H

#include <list>
#include <map>
#include <string>
#include <vector>

#include "libpixel/graphics/render/renderer.h"
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
    
private:
    Vec2 _Offset;
    Vec2 _Position;
    Vec3 _Rotation;
    Vec2 _Scale;
    BlendMode _BlendMode;
    
	Sprite* _Sprite;
    
    friend class SpriteRenderer;
};

class SpriteRenderer
{
public:
	SpriteRenderer();
	~SpriteRenderer();
    
    void Render();
	
	bool Load(const std::string& spriteSheet, bool generateMips);
    bool Unload(const std::string& spriteSheet);
    
    bool AttachToRenderer(const std::string& sheetName, const std::string& spriteName, Vec2 position, Vec3 rotation = Vec3(0.f, 0.f, 0.f), Vec2 scale = Vec2(1.f, 1.f), BlendMode blendMode = kBlendModeNormal, Vec2 offset = Vec2(0.f, 0.f));
    
    Sprite* GetSprite(const std::string& sheetName, const std::string& spriteName) const;
		
private:
    SpriteSheet* GetSpriteSheet(const std::string& spriteSheet) const;
    
	typedef std::list<SpriteInstance> InstanceList;
    typedef std::map<std::string, SpriteSheet*> SheetMap;
	
	InstanceList _Instances;
    
	SheetMap _SpriteSheets;
    
private:
    Vertex _Vertices[4];
    GLuint _VertexBuffer;
    GLuint _IndexBuffer;
};
    
}

#endif