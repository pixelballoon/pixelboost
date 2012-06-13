#pragma once

#include "glm/glm.hpp"
#include "pixelboost/db/definitions.h"

namespace pb
{
    
class Effect;
class Viewport;
    
class Renderable
{
public:
    Renderable(Uid entityUid);
    Renderable(Uid entityUid, Effect* effect);
    virtual ~Renderable();
    
    Uid GetEntityUid();
    
    virtual Uid GetRenderableType() = 0;
    
    virtual void SetLayer(int layer);
    virtual int GetLayer();
    
    virtual void CalculateMVP(Viewport* viewport) = 0;    
    const glm::mat4x4& GetMVP() const;
    
    virtual Effect* GetEffect();
    
protected:
    glm::mat4x4 _MVPMatrix;
    
private:
    Uid _EntityUid;
    int _Layer;
    Effect* _Effect;
};
    
}
