#ifndef PIXELBOOST_DISABLE_BOX2D

#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/renderer/box2d/box2dRenderer.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

Box2dRenderer::Box2dRenderer()
    : _Layer(0)
    , _Scene(0)
{
    SetFlags(b2Draw::e_shapeBit|b2Draw::e_jointBit|b2Draw::e_aabbBit|b2Draw::e_pairBit|b2Draw::e_centerOfMassBit);
}

Box2dRenderer::~Box2dRenderer()
{
    
}
    
void Box2dRenderer::SetLayer(int layer)
{
    _Layer = layer;
}

void Box2dRenderer::SetScene(Scene* scene)
{
    _Scene = scene;
    _DebugRenderSystem = _Scene->GetSystemByType<DebugRenderSystem>();
}

void Box2dRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    if (!_DebugRenderSystem)
        return;

    for (int i=0; i<vertexCount-1; i++)
    {
        _DebugRenderSystem->AddLine(pb::kRenderPassScene, _Layer, glm::vec3(vertices[i].x, vertices[i].y, 0), glm::vec3(vertices[i+1].x, vertices[i+1].y, 0), glm::vec4(color.r, color.g, color.b, 1.f));
    }
    
    if (vertexCount)
    {
        _DebugRenderSystem->AddLine(pb::kRenderPassScene, _Layer, glm::vec3(vertices[vertexCount-1].x, vertices[vertexCount-1].y, 0), glm::vec3(vertices[0].x, vertices[0].y, 0), glm::vec4(color.r, color.g, color.b, 1.f));
    }
}

void Box2dRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    DrawPolygon(vertices, vertexCount, color);
}

void Box2dRenderer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    if (!_DebugRenderSystem)
        return;
    
    _DebugRenderSystem->AddEllipse(pb::kRenderPassScene, _Layer, glm::vec3(center.x, center.y, 0), glm::vec3(0,0,0), glm::vec2(radius, radius), glm::vec4(color.r, color.g, color.b, 1.f));
}

void Box2dRenderer::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    if (!_DebugRenderSystem)
        return;
    
    DrawCircle(center, radius, color);
    
    _DebugRenderSystem->AddLine(pb::kRenderPassScene, _Layer, glm::vec3(center.x, center.y, 0), glm::vec3(center.x+axis.x, center.y+axis.y, 0), glm::vec4(color.r, color.g, color.b, 1.f));
}

void Box2dRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    if (!_DebugRenderSystem)
        return;
    
    _DebugRenderSystem->AddLine(pb::kRenderPassScene, _Layer, glm::vec3(p1.x, p1.y, 0), glm::vec3(p2.x, p2.y, 0), glm::vec4(color.r, color.g, color.b, 1.f));
}

void Box2dRenderer::DrawTransform(const b2Transform& xf)
{
    if (!_DebugRenderSystem)
        return;
    
    _DebugRenderSystem->AddLine(pb::kRenderPassScene, _Layer, glm::vec3(xf.p.x, xf.p.y, 0), glm::vec3(xf.p.x + cos(xf.q.GetAngle())*2.f, xf.p.y + sin(xf.q.GetAngle())*2.f, 0));
}

#endif
