#pragma once

#ifndef PIXELBOOST_DISABLE_BOX2D

#include <vector>

#include "Box2d/Box2D.h"
#include "pixelboost/math/maths.h"

class b2Body;
class b2PolygonShape;
class b2World;

namespace pb
{
    struct FixtureDefinition
    {
        std::vector<b2PolygonShape> shapeList;
    };
    
    class Box2DHelpers
    {
    public:
        static FixtureDefinition LoadDefinition(const std::string& filename);
        static b2Body* CreateBodyFromDefinition(b2World* world, const FixtureDefinition& definition, const Vec2& position, void* userData, Vec2 scale=Vec2(1,1));
    };
}

#endif
