#pragma once

#include <map>
#include <string>

namespace pb
{
    class RenderLayer;
}

namespace pixeleditor
{
    class Entity;
    class ViewProperty;
    
    class ViewEntity
    {
    public:
        ViewEntity(Entity* entity);
        ~ViewEntity();
        
        void Update(float time);
        void Render(pb::RenderLayer* layer);
        
    private:
        void ParseProperties();
        
    private:
        typedef std::map<std::string, ViewProperty*> PropertyMap;
        
    private:
        Entity* _Entity;
        PropertyMap _Properties;
    };
}
