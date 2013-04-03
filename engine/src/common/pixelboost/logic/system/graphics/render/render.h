#pragma once

#include "pixelboost/logic/system.h"

namespace pb
{
    
    class Renderable;

    class RenderSystem : public SceneSystem
    {
    public:
        virtual Uid GetType() const;
        static Uid GetStaticType();
        
        virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);
        
        virtual void AddItem(Renderable* renderable);
        virtual void RemoveItem(Renderable* renderable);
        
        virtual void UpdateBounds(Renderable* renderable);
        
        virtual const std::set<Renderable*>& GetItems(RenderPass pass) = 0;

    protected:
        void RenderItem(Renderable* renderable);
        
        Uid _RenderFilter;
    };
    
}
