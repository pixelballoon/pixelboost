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
        
        virtual void CalculateMVP(Viewport* viewport);
        
        virtual Effect* GetEffect();
        
        void SetFont(const std::string& font);
        const std::string& GetFont();
        
        void SetText(const std::string& text);
        const std::string& GetText();
        
        void SetTint(const glm::vec4& tint);
        const glm::vec4& GetTint();
        
        void SetSize(float size);
        float GetSize();
        
        void SetTransform(const glm::mat4x4& transform);
        const glm::mat4x4& GetTransform();
        
        void SetAlignment(FontAlign alignment);
        FontAlign GetAlignment();
        
    private:
        void CalculateOffset();
        
        float Offset;
        std::string Font;
        std::string Text;
        glm::mat4x4 Transform;
        FontAlign Alignment;
        glm::vec4 Tint;
        float Size;
        
        friend class FontRenderer;
    };
    
    class Font
    {
    public:
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
        
        float FillVertexBuffer(VertexBuffer* vertexBuffer, const std::string& string);
        
    private:
        void AddCharacter(Vertex_PXYZ_UV* buffer, const Font::Character& character, float offset, float baseline);
    };
    
    class FontRenderer : public IRenderer
    {
    public:
        FontRenderer(int maxCharacters=128);
        virtual ~FontRenderer();
        
        Font* LoadFont(const std::string& name, bool createMips=true);
        
        Font* GetFont(const std::string& name);
        
        virtual void Render(int count, Renderable** renderables, Viewport* viewport, EffectPass* effectPass);
        
        float MeasureString(const std::string& name, const std::string& string, float size);
    
    private:
        void SplitString(const std::string& string, char seperator, std::vector<std::string>& output);
        
        typedef std::map<std::string, Font*> FontMap;
        
        FontMap _Fonts;
        
        int _MaxCharacters;
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
    };
    
}
