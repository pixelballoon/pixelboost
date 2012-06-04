#pragma once

#include "pixelboost/db/definitions.h"

namespace pb
{
    
class Material;
    
class Renderable
{
public:
    Renderable();
    Renderable(Material* material);
    virtual ~Renderable();
    
    virtual Uid GetRenderableType() = 0;
    
    virtual void SetLayer(int layer);
    virtual int GetLayer();
    
    Material* GetMaterial();
    
private:
    int _Layer;
    Material* _Material;
};
    
}
