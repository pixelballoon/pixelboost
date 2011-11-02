#pragma once

#include <map>
#include <string>
#include <vector>

#include "libpixel/math/maths.h"

namespace libpixel
{
    class IndexBuffer;
    class Texture;
    class VertexBuffer;
    struct Vertex_PXYZ_UV;
    
    enum FontAlign
    {
        kFontAlignLeft,
        kFontAlignCenter,
        kFontAlignRight,
    };
    
    class FontRenderer
    {
    public:
        FontRenderer(int maxCharacters=128);
        ~FontRenderer();
        
        void LoadFont(const std::string& fontName);
        
        void Render();
        
        bool AttachToRenderer(const std::string& fontName, const std::string& string, Vec2 position, FontAlign alignment = kFontAlignCenter, float scale = 32.f, float rotation = 0.f);
        
        void SplitString(const std::string& string, char seperator, std::vector<std::string>& output);
        
    private:
        struct FontInstance
        {
            std::string _Font;
            std::string _String;
            Vec2 _Position;
            FontAlign _Alignment;
            float _Rotation;
            float _Scale;        
        };
        
        struct Font
        {
            struct Character
            {
                float x;
                float y;
                float width;
                float height;
                float xOffset;
                float yOffset;
                float xAdvance;
            };
            
            Texture* texture;
            float base;
            float lineHeight;
            std::map<char, Character> chars;
            std::map<std::pair<char, char>, float> kerning;
        };
        
        void AddCharacter(Vertex_PXYZ_UV* buffer, const Font::Character& character, float offset, float baseline);
        
        typedef std::map<std::string, Font*> FontMap;
        typedef std::vector<FontInstance> InstanceList;
        
        FontMap _Fonts;
        InstanceList _Instances;

        int _MaxCharacters;
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
    };
    
}
