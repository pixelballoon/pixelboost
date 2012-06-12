#ifndef PIXELBOOST_DISABLE_BOX2D

#include "pixelboost/data/json/reader.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/physics/2d/helpers.h"

#include "Box2D/Box2D.h"

using namespace pb;

FixtureCollection2D* PhysicsHelpers2D::LoadDefinition(const std::string& filename)
{
    std::string definitionString = pb::FileHelpers::FileToString(pb::FileHelpers::GetRootPath()+"/data/physics/"+filename+".phy");
    
    json::Array fixtures;
    
    if (!json::Reader::Read(fixtures, definitionString))
        return 0;
    
    FixtureCollection2D* fixtureCollection = new FixtureCollection2D();
    
    for (json::Array::iterator fixtureIt = fixtures.Begin(); fixtureIt != fixtures.End(); ++fixtureIt)
    {
        json::Object fixture = *fixtureIt;
        
        FixtureDefinition2D fixtureDefinition;
        
        json::Array& hulls = fixture["hulls"];
        
        for (json::Array::iterator hullIt = hulls.Begin(); hullIt != hulls.End(); ++hullIt)
        {
            json::Array& elements = *hullIt;
            
            if (elements.Size() >= 3)
            {
                b2PolygonShape shape;
            
                b2Vec2* vertices = new b2Vec2[elements.Size()];
                
                int i=0;
                for (json::Array::iterator pointIt = elements.Begin(); pointIt != elements.End(); ++pointIt)
                {
                    json::Array& pt = *pointIt;
                    vertices[i].x = static_cast<json::Number>(pt[0]).Value()/32.f;
                    vertices[i].y = static_cast<json::Number>(pt[1]).Value()/32.f;
                    i++;
                }
                
                shape.Set(vertices, elements.Size());
                delete[] vertices;
                
                fixtureDefinition.Shapes.push_back(shape);
            }
        }
        
        fixtureCollection->Definitions[(json::String)fixture["name"]] = fixtureDefinition;
    }
    
    return fixtureCollection;
}

b2Body* PhysicsHelpers2D::CreateBodyFromDefinition(b2World* world, const FixtureDefinition2D& fixtureDef, const glm::vec2& position, void* userData, glm::vec2 scale)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(position.x, position.y);
    bodyDef.userData = userData;
    
    b2PolygonShape poly;
    
    b2Body* body = world->CreateBody(&bodyDef);
    
    for (std::vector<b2PolygonShape>::const_iterator it = fixtureDef.Shapes.begin(); it != fixtureDef.Shapes.end(); ++it)
    {
        poly = *it;
        for (int i=0; i<b2_maxPolygonVertices; i++)
        {
            poly.m_vertices[i] = b2Vec2(poly.m_vertices[i].x*scale.x, poly.m_vertices[i].y*scale.y);
        }
        
        body->CreateFixture(&poly, 0.f);
    }
    
    return body;
}

#endif