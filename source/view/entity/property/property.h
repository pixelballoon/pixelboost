#pragma once

namespace pb
{
    class RenderLayer;
}

namespace pixeleditor
{
    class ViewEntity;
    
    class ViewProperty
    {
    public:
        ViewProperty(ViewEntity* parent);
        virtual ~ViewProperty();
        
        virtual void Update(float time);
        virtual void Render(pb::RenderLayer* layer);
        
    protected:
        ViewEntity* _Parent;
    };
}
