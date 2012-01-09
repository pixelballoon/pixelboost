#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/graphics/render/common/irenderer.h"

#include "pixelboost/math/maths.h"

namespace pixelboost
{
    class IndexBuffer;
    class Renderer;
    class Texture;
    class VertexBuffer;
    struct Vertex_PXYZ_UV;
    
    enum FontAlign
    {
        kFontAlignLeft,
        kFontAlignCenter,
        kFontAlignRight,
    };
    
    class FontRenderer : public IRenderer
    {
    public:
        FontRenderer(Renderer* renderer, int maxCharacters=128);
        virtual ~FontRenderer();
        
        void LoadFont(const std::string& fontName);
        
        virtual void Update(float time);
        virtual void Render(RenderLayer* layer);
        
        bool AttachToRenderer(RenderLayer* layer, const std::string& fontName, const std::string& string, Vec2 position, FontAlign alignment = kFontAlignCenter, float scale = 32.f, float rotation = 0.f, Vec4 color = Vec4(1.f, 1.f, 1.f, 1.f));
        
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
            Vec4 _Color;
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
        typedef std::map<RenderLayer*, InstanceList> InstanceListMap;
        
        FontMap _Fonts;
        InstanceListMap _Instances;

        int _MaxCharacters;
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
    };
    
}
