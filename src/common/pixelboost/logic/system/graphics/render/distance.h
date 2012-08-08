#pragma once

#include <set>

#include "pixelboost/logic/system/graphics/render/render.h"

namespace pb
{
    
class Renderable;

class DistanceRenderSystem : public RenderSystem
{
public:
    DistanceRenderSystem(float distance);
    
    static Uid GetStaticType();
    
    virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);
    
    virtual void AddItem(Renderable* renderable);
    virtual void RemoveItem(Renderable* renderable);
    
private:
    typedef std::set<Renderable*> RenderableSet;
    
    RenderableSet _SceneRenderables;
    RenderableSet _UiRenderables;
    
    float _Distance;
};
    
}
