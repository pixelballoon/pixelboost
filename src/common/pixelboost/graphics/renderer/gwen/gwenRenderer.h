#pragma once

#ifndef PIXELBOOST_DISABLE_GRAPHICS

#ifndef PIXELBOOST_DISABLE_GWEN

#include "Gwen/Gwen.h"
#include "gwen/BaseRender.h"

#include "glm/glm.hpp"

namespace pb
{
    class RenderLayer;
    
    class GwenRenderer : public Gwen::Renderer::Base
    {
    public:
        GwenRenderer(pb::RenderLayer* layer);
        ~GwenRenderer();
        
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
        RenderLayer* _Layer;
        
        glm::vec4 _Colour;
    };
}

#endif

#endif