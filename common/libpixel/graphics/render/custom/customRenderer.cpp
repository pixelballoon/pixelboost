#include "libpixel/graphics/device/device.h"
#include "libpixel/graphics/render/custom/customRenderer.h"

namespace libpixel
{

CustomRendererItem::~CustomRendererItem()
{

}
    
CustomRenderer::CustomRenderer()
{
    
}
    
CustomRenderer::~CustomRenderer()
{
    
}

void CustomRenderer::Update(float time)
{
    _Items.clear();
}

void CustomRenderer::Render(RenderLayer* layer)
{
    ItemList& list = _Items[layer];
    
    if (!list.size())
        return;
    
    for (ItemList::iterator it = list.begin(); it != list.end(); ++it)
    {
        (*it)->OnCustomRender();
    }
}

void CustomRenderer::AttachItem(RenderLayer* layer, CustomRendererItem* item)
{
    _Items[layer].push_back(item);
}
    
}
