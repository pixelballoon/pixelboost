#ifndef LIBPIXEL__GRAPHICS__RENDER__FONT__FONTRENDERER__H
#define LIBPIXEL__GRAPHICS__RENDER__FONT__FONTRENDERER__H

#include <list>
#include <map>
#include <string>

#include "libpixel/math/maths.h"

class Font;

namespace libpixel
{
    
class Texture;

enum FontAlign
{
    kFontAlignLeft,
    kFontAlignCenter,
    kFontAlignRight,
};

struct FontInstance
{
    std::string _Font;
    std::string _String;
    Vec2 _Position;
    FontAlign _Alignment;
    float _Rotation;
    float _Scale;        
};

class FontRenderer
{
public:
    FontRenderer();
    ~FontRenderer();
    
    void Render();
    
    bool AttachToRenderer(const std::string& fontName, const std::string& string, Vec2 position, FontAlign alignment = kFontAlignCenter, float rotation = 0.f, float scale = 1.f);
    
private:
    struct Font
    {
        Font();
        ~Font();
        
        struct Character
        {
            float size;
            float offset;
        };
        
        typedef std::map<char, Character> CharacterMap;
        
        CharacterMap characterMap;
        Texture* texture;
    };
    
    typedef std::map<std::string, Font*> FontMap;
    typedef std::list<FontInstance> InstanceList;
    
    FontMap _Fonts;
    InstanceList _Instances;
};
    
}

#endif