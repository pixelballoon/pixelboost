#pragma once

#include <map>
#include <vector>

#include "glm/glm.hpp"
#include "sigslot/delegate.h"

#include "pixelboost/framework/definitions.h"
#include "pixelboost/graphics/renderer/common/renderable.h"

namespace pb
{

class Camera;
class OrthographicCamera;
class Renderable;
class RenderLayer;
class Scene;
class Shader;
class ShaderTechnique;
    
class Viewport
{
public:
    Viewport(int viewportId, Camera* camera);
    ~Viewport();
    
    glm::vec4 GetNativeRegion();
    glm::vec2 GetSize();
    
    void SetResolution(glm::vec2 size);
    glm::vec2 GetResolution();
    
    void SetDensity(float density);
    float GetDensity();
    
    void SetPosition(glm::vec2 position);
    glm::vec2 GetPosition();
    
    void SetSceneCamera(Camera* camera);
    Camera* GetSceneCamera();
    
    OrthographicCamera* GetUiCamera();
    
    void SetRenderScheme(Uid scheme);
    Uid GetRenderScheme();
    
    void SetScene(Scene* scene);
    Scene* GetScene();
    
    void Render(RenderPass renderPass);

private:
    Camera* _SceneCamera;
    OrthographicCamera* _UiCamera;
    Uid _RenderScheme;
    Scene* _Scene;
    
    int _ViewportId;
    glm::vec2 _Position;
    glm::vec2 _Resolution;
    float _Density;
    
    friend class Renderer;
};

}
