#pragma once

#include <map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/file/fileSystem.h"
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
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Effect* GetEffect();
        
        void SetFont(const std::string& font);
        const std::string& GetFont();
        
        void SetText(const std::wstring& text);
        const std::wstring& GetText();
        
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
        std::wstring Text;
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
        std::map<wchar_t, Character> chars;
        std::map<std::pair<wchar_t, wchar_t>, float> kerning;
        
        glm::vec2 FillVertexBuffer(VertexBuffer* vertexBuffer, const std::wstring& string);
        
    private:
        void AddCharacter(Vertex_PXYZ_UV* buffer, const Font::Character& character, glm::vec2 offset, float baseline);
    };
    
    class FontRenderer : public IRenderer
    {
    public:
        FontRenderer(int maxCharacters=1024);
        virtual ~FontRenderer();
        
        Font* LoadFont(FileLocation location, const std::string& name, const std::string& filename, bool createMips=true);
        
        Font* GetFont(const std::string& name);
        
        virtual void Render(int count, Renderable** renderables, Viewport* viewport, EffectPass* effectPass);
        
        glm::vec2 MeasureString(const std::string& name, const std::wstring& string, float size);
    
    private:
        void SplitString(const std::wstring& string, wchar_t seperator, std::vector<std::wstring>& output);
        
        typedef std::map<std::string, Font*> FontMap;
        
        FontMap _Fonts;
        
        int _MaxCharacters;
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
    };
    
}
