#pragma once

#include <map>
#include <vector>

#include "glm/glm.hpp"

namespace pb
{

class Camera;
class RenderLayer;
class Scene;
    
class Viewport
{
public:
    Viewport(int viewportId);
    ~Viewport();
    
    glm::vec2 GetSize();
    
    void SetResolution(glm::vec2 size);
    glm::vec2 GetResolution();
    
    void SetDensity(float density);
    float GetDensity();
    
    void SetPosition(glm::vec2 position);
    glm::vec2 GetPosition();
    
    void SetScene(Scene* scene);
    Scene* GetScene();
    
    void AddLayer(RenderLayer* layer);
    void RemoveLayer(RenderLayer* layer);
    
    void Render();

private:
    typedef std::vector<RenderLayer*> LayerList;
    
    Scene* _Scene;
    
    int _ViewportId;
    glm::vec2 _Position;
    glm::vec2 _Resolution;
    float _Density;
    
    LayerList _Layers;
};

}
