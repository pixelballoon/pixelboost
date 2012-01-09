#pragma once

#include <map>
#include <vector>

#include "pixelboost/graphics/render/common/irenderer.h"
#include "pixelboost/math/maths.h"

namespace pixelboost
{
    
class Renderer;

class PrimitiveRenderer : public IRenderer
{
public:
    PrimitiveRenderer(Renderer* renderer);
    ~PrimitiveRenderer();
    
    void Update(float time);
    void Render(RenderLayer* layer);
    
    void AttachEllipse(RenderLayer* layer, Vec2 position, Vec2 size, Vec3 rotation = Vec3(0.f, 0.f, 0.f), Vec4 color = Vec4(1.f, 1.f, 1.f, 1.f), int segments = 12);
    void AttachLine(RenderLayer* layer, Vec2 start, Vec2 end, Vec4 color = Vec4(1.f, 1.f, 1.f, 1.f));
    void AttachBox(RenderLayer* layer, Vec2 position, Vec2 size, Vec3 rotation = Vec3(0.f, 0.f, 0.f), Vec4 color = Vec4(1.f, 1.f, 1.f, 1.f), bool solid=true);
    
private:
    struct PrimitiveInstance
    {
        enum Type
        {
            kTypeEllipse,
            kTypeLine,
            kTypeBox,
        };
        
        Type type;
        Vec2 position;
        Vec2 size;
        Vec3 rotation;
        Vec4 color;
        bool solid;
        int segments;
    };
    
    typedef std::vector<PrimitiveInstance> ItemList;
    typedef std::map<RenderLayer*, ItemList> ItemListMap;
    
    ItemListMap _Items;
};
    
}
