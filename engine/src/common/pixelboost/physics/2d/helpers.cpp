#ifndef PIXELBOOST_DISABLE_BOX2D

#include "json/reader.h"

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/physics/2d/helpers.h"

#include "Box2D/Box2D.h"

using namespace pb;

FixtureCollection2D* PhysicsHelpers2D::LoadDefinition(const std::string& filename, float density)
{
    pb::File* file = pb::FileSystem::Instance()->OpenFile("/physics/"+filename+".phy");
    
    std::string definitionString;
    
    if (file)
    {
        file->ReadAll(definitionString);
        delete file;
    }
    
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
                    vertices[i].x = static_cast<json::Number>(pt[0]).Value()/density;
                    vertices[i].y = static_cast<json::Number>(pt[1]).Value()/density;
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

b2Body* PhysicsHelpers2D::CreateBodyFromDefinition(b2World* world, b2BodyDef bodyDef, const FixtureDefinition2D& fixtureDef, float density, glm::vec2 scale)
{
    b2PolygonShape poly;
    
    b2Body* body = world->CreateBody(&bodyDef);
    
    for (std::vector<b2PolygonShape>::const_iterator it = fixtureDef.Shapes.begin(); it != fixtureDef.Shapes.end(); ++it)
    {
        poly = *it;
        for (int i=0; i<poly.m_vertexCount; i++)
        {
            poly.m_vertices[i] = b2Vec2(poly.m_vertices[i].x*scale.x, poly.m_vertices[i].y*scale.y);
        }
        
        body->CreateFixture(&poly, density);
    }
    
    return body;
}

#endif
