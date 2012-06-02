#pragma once

#ifndef PIXELBOOST_DISABLE_BOX2D

#include <vector>

#include "Box2d/Box2D.h"
#include "glm/glm.hpp"

class b2Body;
class b2PolygonShape;
class b2World;

namespace pb
{
    class Box2DHelpers
    {
    public:
        typedef std::vector<b2PolygonShape> FixtureDefinition;
        typedef std::map<std::string, FixtureDefinition> FixtureCollection;
        
        static FixtureCollection LoadDefinition(const std::string& filename);
        static b2Body* CreateBodyFromDefinition(b2World* world, const FixtureDefinition& definition, const glm::vec2& position, void* userData, glm::vec2 scale=glm::vec2(1,1));
    };
}

#endif
