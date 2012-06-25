#pragma once

#include "view/entity/property/property.h"

namespace pb
{
    class ModelComponent;
    class RenderLayer;
}

namespace pixeleditor
{
    class ModelViewProperty : public ViewProperty
    {
    public:
        ModelViewProperty(ViewEntity* parent, const std::string& path, const SchemaItem* item);
        virtual ~ModelViewProperty();
        
        virtual void Update(float time);
        virtual void Render(int layer);
        
        virtual void Refresh();
        
    protected:
        virtual pb::BoundingBox CalculateBounds();
        
    private:
        std::string _Model;
        std::string _Texture;
        
        pb::ModelComponent* _ModelComponent;
    };
}
