#pragma once

namespace pb
{

class RenderLayer;
    
class IRenderer
{
public:
    IRenderer();
    virtual ~IRenderer();
    
    int GetId();
    
    virtual void Update(float time) = 0;
    virtual void Render(RenderLayer* layer) = 0;
    
private:
    int _Id;
};

}
