#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/irenderer.h"

namespace pb
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
    
    class FontRenderer : public IRenderer
    {
    public:
        FontRenderer(int maxCharacters=128);
        virtual ~FontRenderer();
        
        void LoadFont(const std::string& fontName, bool createMips=true);
        
        virtual void Update(float time);
        virtual void Render(int count, Renderable** renderables, Viewport* viewport, EffectPass* effectPass);
        
        //bool AttachToRenderer(RenderLayer* layer, const std::string& fontName, const std::string& string, glm::vec2 position, FontAlign alignment = kFontAlignCenter, float size = 1.f, float rotation = 0.f, glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f));
        
        float MeasureString(const std::string& fontName, const std::string& string, float size);
    
    private:
        void SplitString(const std::string& string, char seperator, std::vector<std::string>& output);
        
        struct FontInstance
        {
            std::string _Font;
            std::string _String;
            glm::vec2 _Position;
            FontAlign _Alignment;
            float _Rotation;
            float _Size;
            glm::vec4 _Color;
        };
        
        struct Font
        {
            struct Character
            {
                float uvx;
                float uvy;
                float uvu;
                float uvv;
                float width;
                float height;
                float xOffset;
                float yOffset;
                float xAdvance;
            };
            
            Texture* texture;
            int size;
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
