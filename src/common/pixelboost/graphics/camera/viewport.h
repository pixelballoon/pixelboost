#pragma once

#include <map>
#include <vector>

#include "glm/glm.hpp"

#include "pixelboost/db/definitions.h"

namespace pb
{

class Camera;
class RenderLayer;
class Scene;
    
class Viewport
{
public:
    Viewport(int viewportId, Camera* camera);
    ~Viewport();
    
    glm::vec2 GetSize();
    
    void SetResolution(glm::vec2 size);
    glm::vec2 GetResolution();
    
    void SetDensity(float density);
    float GetDensity();
    
    void SetPosition(glm::vec2 position);
    glm::vec2 GetPosition();
    
    Camera* GetCamera();
    
    void SetRenderScheme(Uid scheme);
    Uid GetRenderScheme();
    
    void SetScene(Scene* scene);
    Scene* GetScene();
    
    void Render();

private:
    Camera* _Camera;
    Uid _RenderScheme;
    Scene* _Scene;
    
    int _ViewportId;
    glm::vec2 _Position;
    glm::vec2 _Resolution;
    float _Density;
};

}
