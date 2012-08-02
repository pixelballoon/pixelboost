#pragma once

#include "pixelboost/logic/system.h"

namespace pb
{
    
class Renderable;

class RenderSystem : public SceneSystem
{
public:
    virtual Uid GetType();
    static Uid GetStaticType();
    
    virtual void AddItem(Renderable* renderable);
    virtual void RemoveItem(Renderable* renderable);
    
    virtual void UpdateBounds(Renderable* renderable);

protected:
    void RenderItem(Renderable* renderable);
};
    
}
