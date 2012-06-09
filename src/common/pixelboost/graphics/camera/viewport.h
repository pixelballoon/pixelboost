#pragma once

#include <map>
#include <vector>

#include "glm/glm.hpp"
#include "sigslot/delegate.h"

#include "pixelboost/db/definitions.h"

namespace pb
{

class Camera;
class Effect;
class EffectTechnique;
class Renderable;
class RenderLayer;
class Scene;
class Technique;
    
class Viewport
{
public:
    Viewport(int viewportId, Camera* camera);
    ~Viewport();
    
    glm::vec2 GetSize();
    
    void SetTechniqueDelegate(sigslot::Delegate2<Renderable*, Effect*, EffectTechnique*> delegate);
    
    void SetResolution(glm::vec2 size);
    glm::vec2 GetResolution();
    
    void SetDensity(float density);
    float GetDensity();
    
    void SetPosition(glm::vec2 position);
    glm::vec2 GetPosition();
    
    void SetCamera(Camera* camera);
    Camera* GetCamera();
    
    void SetRenderScheme(Uid scheme);
    Uid GetRenderScheme();
    
    void SetScene(Scene* scene);
    Scene* GetScene();
    
    void Render();

private:
    EffectTechnique* GetTechnique(Renderable* renderable, Effect* effect);
    
    sigslot::Delegate2<Renderable*, Effect*, EffectTechnique*> _TechniqueDelegate;
    
    Camera* _Camera;
    Uid _RenderScheme;
    Scene* _Scene;
    
    int _ViewportId;
    glm::vec2 _Position;
    glm::vec2 _Resolution;
    float _Density;
    
    friend class Renderer;
};

}
