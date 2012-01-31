#include "pixelboost/data/json/reader.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/physics/helpers.h"

#include "Box2D/Box2D.h"

using namespace pixelboost;

FixtureDefinition Box2DHelpers::LoadDefinition(const std::string& filename)
{
    std::string definitionString = pixelboost::FileHelpers::FileToString(pixelboost::FileHelpers::GetRootPath()+"/data/physics/"+filename+".phy");
    
    json::Object definition;
    
    if (!json::Reader::Read(definition, definitionString))
        return FixtureDefinition();
    
    FixtureDefinition fixtureDefinition;
    
    json::Array& hulls = definition["hulls"];
    
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
                json::Object& pt = *pointIt;
                vertices[i].x = static_cast<json::Number>(pt["x"]).Value();
                vertices[i].y = static_cast<json::Number>(pt["y"]).Value();
                i++;
            }
            
            shape.Set(vertices, elements.Size());
            delete[] vertices;
            
            fixtureDefinition.shapeList.push_back(shape);
        }
    }
    
    return fixtureDefinition;
}

b2Body* Box2DHelpers::CreateBodyFromDefinition(b2World* world, const FixtureDefinition& fixtureDef, const Vec2& position, void* userData, Vec2 scale)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = b2Vec2(position[0], position[1]);
    bodyDef.userData = userData;
    
    b2PolygonShape poly;
    
    b2Body* body = world->CreateBody(&bodyDef);
    
    for (std::vector<b2PolygonShape>::const_iterator it = fixtureDef.shapeList.begin(); it != fixtureDef.shapeList.end(); ++it)
    {
        poly = *it;
        for (int i=0; i<b2_maxPolygonVertices; i++)
        {
            poly.m_vertices[i] = b2Vec2(poly.m_vertices[i].x*scale[0], poly.m_vertices[i].y*scale[1]);
        }
        
        body->CreateFixture(&poly, 0.f);
    }
    
    return body;
}
