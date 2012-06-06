#include "pixelboost/logic/system.h"

namespace pb
{
    
class Renderable;

class RenderSystem : public SceneSystem
{
public:
    virtual Uid GetType();
    static Uid GetStaticType();
    
    virtual void AddItem(Renderable* renderable) = 0;
    virtual void RemoveItem(Renderable* renderable) = 0;

};
    
}
