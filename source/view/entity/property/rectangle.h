#pragma once

#include "view/entity/property/property.h"

namespace pb
{
    class RectangleComponent;
}

namespace pixeleditor
{
    class RectangleViewProperty : public ViewProperty
    {
    public:
        RectangleViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* item);
        virtual ~RectangleViewProperty();
        
        virtual void Update(float time);
        virtual void Render(int layer);
        
        virtual void Refresh();
        
    protected:
        virtual pb::BoundingBox CalculateBounds();
    };
}
