#pragma once

#include "view/entity/property/property.h"

namespace pb
{
    class SpriteComponent;
}

class SpriteViewProperty : public ViewProperty
{
public:
    SpriteViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* item);
    virtual ~SpriteViewProperty();
    
    virtual void Update(float time);
    virtual void Render(int layer);
    
    virtual void Refresh();
    
protected:
    virtual pb::BoundingBox CalculateBounds();
    
private:
    std::string _Sprite;
};
