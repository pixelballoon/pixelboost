#pragma once

#ifndef PIXELBOOST_DISABLE_BOX2D

#include "Box2d/Box2d.h"

namespace pb
{
    
    class DebugRenderSystem;
    class Scene;

    class Box2dRenderer : public b2Draw
    {
    public:
        Box2dRenderer();
        virtual ~Box2dRenderer();
        
        void SetLayer(int layer);
        void SetScene(Scene* scene);
        
        virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
        virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
        virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
        virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
        virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
        virtual void DrawTransform(const b2Transform& xf);
        
    private:
        int _Layer;
        
        Scene* _Scene;
        DebugRenderSystem* _DebugRenderSystem;
    };
    
}

#endif
