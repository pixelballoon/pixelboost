#pragma once

#ifdef PIXELBOOST_LIBRARY_USE_GWEN

#include "Gwen/Gwen.h"
#include "Gwen/BaseRender.h"

#include "glm/glm.hpp"

#include "pixelboost/graphics/renderer/common/irenderer.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

namespace pb
{
    class IndexBuffer;
    class RenderLayer;
    class VertexBuffer;
    struct Vertex_P3_UV;
    
    class GwenRenderable : public pb::Renderable
    {
    public:
        GwenRenderable(Gwen::Controls::Canvas* canvas);
        virtual ~GwenRenderable();
        
        virtual Uid GetType();
        static Uid GetStaticType();
        
        virtual void CalculateBounds();
        virtual void CalculateWorldMatrix();
        
        virtual Shader* GetShader();
        
    private:
        Gwen::Controls::Canvas* _Canvas;
        
        friend class GwenRenderer;
    };
    
    class GwenRenderer : public Gwen::Renderer::Base, public pb::IRenderer
    {
    public:
        GwenRenderer();
        virtual ~GwenRenderer();
        
        virtual void Render(int count, Renderable** renderables, Viewport* viewport, ShaderPass* shaderPass);
        
    public:
        virtual void Init();
        
        virtual void Begin();
        virtual void End();
        
        virtual void SetDrawColor( Gwen::Color color );
        virtual void DrawFilledRect( Gwen::Rect rect );
        
        void StartClip();
        void EndClip();
        
        void DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1=0.0f, float v1=0.0f, float u2=1.0f, float v2=1.0f);
        void LoadTexture(Gwen::Texture* pTexture);
        void FreeTexture(Gwen::Texture* pTexture);
        Gwen::Color PixelColour(Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default);
        
        virtual void RenderText(Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text);
        virtual Gwen::Point MeasureText(Gwen::Font* pFont, const Gwen::UnicodeString& text);

    private:
        void PurgeBuffer(bool force);
        
        IndexBuffer* _IndexBuffer;
        VertexBuffer* _VertexBuffer;
        IndexBuffer* _FontIndexBuffer;
        VertexBuffer* _FontVertexBuffer;
        Vertex_P3_UV* _VertexData;
        int _VertexCount;
        
        int _MaxQuads;
        Renderable* _Renderable;
        Viewport* _Viewport;
        ShaderPass* _ShaderPass;
        
        glm::vec4 _Color;
    };
}

#endif
