#pragma once

#include <map>
#include <vector>

#include "pixelboost/graphics/render/common/irenderer.h"
#include "pixelboost/math/maths.h"

namespace pixelboost
{

class Renderer;
    
class CustomRendererItem
{
public:
    virtual ~CustomRendererItem();
    virtual void OnCustomRender() = 0;
};

class CustomRenderer : public IRenderer
{
public:
    CustomRenderer(Renderer* renderer);
    ~CustomRenderer();
    
    void Update(float time);
    void Render(RenderLayer* layer);
    
    void AttachItem(RenderLayer* layer, CustomRendererItem* item);
    
private:
    typedef std::vector<CustomRendererItem*> ItemList;
    typedef std::map<RenderLayer*, ItemList> ItemListMap;
    
    ItemListMap _Items;
};
    
}
