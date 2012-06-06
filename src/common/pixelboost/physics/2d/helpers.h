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
        static FixtureCollection2D* LoadDefinition(const std::string& filename);
        static b2Body* CreateBodyFromDefinition(b2World* world, const FixtureDefinition2D& definition, const glm::vec2& position, void* userData, glm::vec2 scale=glm::vec2(1,1));
    };
}

#endif
