#ifndef LIBPIXEL_DISABLE_BOX2D

#include "libpixel/graphics/render/box2d/box2dRenderer.h"
#include "libpixel/graphics/render/primitive/primitiveRenderer.h"
#include "libpixel/logic/game.h"

namespace libpixel
{

Box2dRenderer::Box2dRenderer()
{
    
}

Box2dRenderer::~Box2dRenderer()
{
    
}

void Box2dRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    for (int i=0; i<vertexCount-1; i++)
    {
        libpixel::Game::Instance()->GetPrimitiveRenderer()->RenderLine(Vec2(vertices[i].x, vertices[i].y), Vec2(vertices[i+1].x, vertices[i+1].y), Vec3(color.r, color.g, color.b));
    }
    
    if (vertexCount)
    {
        libpixel::Game::Instance()->GetPrimitiveRenderer()->RenderLine(Vec2(vertices[vertexCount-1].x, vertices[vertexCount-1].y), Vec2(vertices[0].x, vertices[0].y), Vec3(color.r, color.g, color.b));
    }
}

void Box2dRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    DrawPolygon(vertices, vertexCount, color);
}

void Box2dRenderer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    libpixel::Game::Instance()->GetPrimitiveRenderer()->RenderEllipse(Vec2(center.x, center.y), Vec2(radius, radius), Vec3(color.r, color.g, color.b));
}

void Box2dRenderer::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    DrawCircle(center, radius, color);
}

void Box2dRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    libpixel::Game::Instance()->GetPrimitiveRenderer()->RenderLine(Vec2(p1.x, p1.y), Vec2(p2.x, p2.y), Vec3(color.r, color.g, color.b));
}

void Box2dRenderer::DrawTransform(const b2Transform& xf)
{
    
}

}

#endif