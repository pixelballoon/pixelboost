#pragma once

#include "pixelboost/input/touchManager.h"
#include "pixelboost/logic/component.h"

namespace pb
{

class RectTouchComponent : public Component, TouchHandler
{
public:
    RectTouchComponent(Entity* parent);
    virtual ~RectTouchComponent();
    
    Uid GetStaticType();
    virtual Uid GetType();
    
    void SetSize(const glm::vec2& size);
    
private:
    glm::vec3 GetPosition();
    
    virtual bool OnTouchDown(Touch touch);
	virtual bool OnTouchMove(Touch touch);
	virtual bool OnTouchUp(Touch touch);
    
    glm::vec2 _Size;
};

}
