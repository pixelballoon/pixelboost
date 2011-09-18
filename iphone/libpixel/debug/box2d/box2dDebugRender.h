#ifndef IPHONE__DEBUG__BOX2D__BOX2DDEBUGRENDER__H
#define IPHONE__DEBUG__BOX2D__BOX2DDEBUGRENDER__H

#include "box2d/Box2D.h"

class PhysicsRender : public b2DebugDraw
{
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	virtual void DrawTransform(const b2Transform& xf);
};

#endif