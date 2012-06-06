#pragma once

#include "pixelboost/db/definitions.h"

namespace pb
{
    
class Scene;
class Viewport;

class SceneSystem
{
public:
    SceneSystem();
    ~SceneSystem();
    
    virtual pb::Uid GetType() = 0;
    
    virtual void Update(Scene* scene, float time);
    virtual void Render(Scene* scene, Viewport* viewport);
};

}
