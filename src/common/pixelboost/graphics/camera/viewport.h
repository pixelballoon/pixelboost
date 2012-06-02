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
    
    void AddLayer(RenderLayer* layer, Camera* camera);
    void RemoveLayer(RenderLayer* layer);
    
    void Render();

private:
    typedef std::vector<RenderLayer*> LayerList;
    typedef std::map<RenderLayer*, Camera*> CameraMap;
    
    int _ViewportId;
    glm::vec2 _Position;
    glm::vec2 _Size;
    
    LayerList _Layers;
    CameraMap _Cameras;
};

}
