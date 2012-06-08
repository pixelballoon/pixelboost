#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

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
    
    class FontRenderable : public Renderable
    {
    public:
        FontRenderable(Uid entityId);
        ~FontRenderable();
        
        virtual Uid GetRenderableType();
        
        virtual Effect* GetEffect();
        
        std::string Font;
        std::string Text;
        glm::mat4x4 Transform;
        FontAlign Alignment;
        glm::vec4 Tint;
        float Size;
    };
    
    class FontRenderer : public IRenderer
    {
    public:
        FontRenderer(int maxCharacters=128);
        virtual ~FontRenderer();
        
        void LoadFont(const std::string& fontName, bool createMips=true);
        
        virtual void Render(int count, Renderable** renderables, Viewport* viewport, EffectPass* effectPass);
        
        float MeasureString(const std::string& fontName, const std::string& string, float size);
    
    private:
        void SplitString(const std::string& string, char seperator, std::vector<std::string>& output);
        
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
        
        FontMap _Fonts;
        
        int _MaxCharacters;
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
    };
    
}
