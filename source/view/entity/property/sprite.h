#pragma once

#include "view/entity/property/property.h"

namespace pb
{
    class RenderLayer;
}

namespace pixeleditor
{
    class SpriteViewProperty : public ViewProperty
    {
    public:
        SpriteViewProperty(ViewEntity* parent);
        virtual ~SpriteViewProperty();
        
        virtual void Update(float time);
        virtual void Render(pb::RenderLayer* layer);
    };
}
