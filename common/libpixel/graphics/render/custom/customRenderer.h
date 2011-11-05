#pragma once

#include <map>
#include <vector>

#include "libpixel/graphics/render/common/irenderer.h"
#include "libpixel/math/maths.h"

namespace libpixel
{
    
class CustomRendererItem
{
public:
    virtual ~CustomRendererItem();
    virtual void OnCustomRender() = 0;
};

class CustomRenderer : public IRenderer
{
public:
    CustomRenderer();
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
