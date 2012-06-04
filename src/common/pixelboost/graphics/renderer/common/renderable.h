#pragma once

#include "pixelboost/db/definitions.h"

namespace pb
{
    
class Effect;
    
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
    
    virtual Effect* GetEffect();
    
private:
    Uid _EntityUid;
    int _Layer;
    Effect* _Effect;
};
    
}
