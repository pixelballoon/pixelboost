#pragma once

#include <map>
#include <vector>

#include "glm/glm.hpp"

namespace pb
{

class Camera;
class RenderLayer;
    
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
    
    void AddLayer(RenderLayer* layer, Camera* camera);
    void RemoveLayer(RenderLayer* layer);
    
    void Render();

private:
    typedef std::vector<RenderLayer*> LayerList;
    typedef std::map<RenderLayer*, Camera*> CameraMap;
    
    int _ViewportId;
    glm::vec2 _Position;
    glm::vec2 _Resolution;
    float _Density;
    
    LayerList _Layers;
    CameraMap _Cameras;
};

}
