#pragma once

#include <map>
#include <stdint.h>
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
    struct Vertex_P3_C4_UV;
    
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
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Shader* GetShader();
        
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
        std::map<uint32_t, Character> chars;
        std::map<std::pair<uint32_t, uint32_t>, float> kerning;
        
        int FillVertices(Vertex_P3_C4_UV* vertices, const std::string& string, int maxVertices, glm::vec4 color = glm::vec4(1,1,1,1), const glm::mat4x4& transform = glm::mat4x4(), glm::vec2* measuredSize = 0);
        
    private:
        void AddCharacter(Vertex_P3_C4_UV* buffer, const Font::Character& character, glm::vec2 offset, float baseline, glm::vec4 color, const glm::mat4x4& transform);
    };
    
    class FontRenderer : public IRenderer
    {
    public:
        FontRenderer(int maxCharacters=1024);
        virtual ~FontRenderer();
        
        static FontRenderer* Instance();
        
        Font* LoadFont(const std::string& name, const std::string& filename, bool createMips=true, bool hasPremultipliedAlpha=false);
        
        Font* GetFont(const std::string& name);
        
        virtual void Render(int count, Renderable** renderables, Uid renderScheme, const glm::vec4& viewport, const glm::mat4x4& projectionMatrix, const glm::mat4x4& viewMatrix);
        
        float FitString(glm::vec2 region, const std::string& name, const std::string& string, float preferredSize);
        glm::vec2 MeasureString(const std::string& name, const std::string& string, float size);
    
    private:
        void SplitString(const std::string& string, char seperator, std::vector<std::string>& output);
        
        typedef std::map<std::string, Font*> FontMap;
        
        FontMap _Fonts;
        
        int _MaxCharacters;
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
        
        static FontRenderer* _Instance;
    };
    
}
