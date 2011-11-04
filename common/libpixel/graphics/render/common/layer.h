#pragma once

#include <vector>

namespace libpixel
{

class Camera;
class IRenderer;

class RenderLayer
{
public:
    RenderLayer(Camera* camera);
    
    void Render();
    
    void AddRenderer(IRenderer* renderer);
    void RemoveRenderer(IRenderer* renderer);
    
    typedef std::vector<IRenderer*> RendererList;
    const RendererList& GetRenderers() const;
    
private:
    Camera* _Camera;
    RendererList _Renderers;
};

}
