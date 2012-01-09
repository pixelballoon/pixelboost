#pragma once

namespace pixelboost
{

class Renderer;
class RenderLayer;
    
class IRenderer
{
public:
    IRenderer(Renderer* renderer);
    virtual ~IRenderer();
    
    int GetId();
    
    virtual void Update(float time) = 0;
    virtual void Render(RenderLayer* layer) = 0;
    
private:
    int _Id;
    Renderer* _Renderer;
};

}
