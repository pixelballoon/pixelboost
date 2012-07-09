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
    
    virtual void CalculateWorldMatrix() = 0;
    
    void DirtyWorldMatrix();
    void SetWorldMatrix(const glm::mat4x4& worldMatrix);
    void CalculateMVP(Viewport* viewport);
    const glm::mat4x4& GetMVP() const;
    
    virtual Effect* GetEffect();
    void SetEffect(Effect* effect);
    
protected:
    glm::mat4x4 _WorldMatrix;
    
private:
    bool _WorldMatrixDirty;
    glm::mat4x4 _MVPMatrix;
    
private:
    Uid _EntityUid;
    int _Layer;
    Effect* _Effect;
};
    
}
