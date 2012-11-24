#pragma once

#ifndef PIXELBOOST_DISABLE_BOX2D

#include <map>
#include <string>
#include <vector>

#include "Box2d/Box2D.h"
#include "glm/glm.hpp"

class b2Body;
class b2PolygonShape;
class b2World;

namespace pb
{
    class FixtureDefinition2D
    {
    public:
        typedef std::vector<b2PolygonShape> ShapeList;
        ShapeList Shapes;
    };
    
    class FixtureCollection2D
    {
    public:
        typedef std::map<std::string, FixtureDefinition2D> DefinitionMap;
        DefinitionMap Definitions;
    };
    
    class PhysicsHelpers2D
    {
    public:
        static FixtureCollection2D* LoadDefinition(const std::string& filename, float density=32.f);
        static b2Body* CreateBodyFromDefinition(b2World* world, b2BodyDef bodyDef, const FixtureDefinition2D& definition, float density=1.f, glm::vec2 scale=glm::vec2(1,1));
    };
}

#endif
