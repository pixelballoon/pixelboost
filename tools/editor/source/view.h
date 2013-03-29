#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "sigslot/signal.h"

#include "pixelboost/framework/engine.h"
#include "pixelboost/resource/resourceManager.h"

#include "core/selection.h"

namespace pb
{
    class Camera;
    class MaterialResource;
    class Model;
    class OrbitalCamera;
    class OrthographicCamera;
    class RenderLayer;
    class Scene;
    class Viewport;
}

class CommandManager;
class Core;
class Level;
class ManipulatorManager;
class Project;
class ProjectRecord;
class ViewEntity;
class ViewKeyboardHandler;
class ViewMouseHandler;

class Viewport
{
public:
    enum ViewportType
    {
        kViewportTypeOrthographic,
        kViewportTypePerspective,
    };
    
    virtual ViewportType GetType() = 0;
    virtual pb::Camera* GetCamera() = 0;
    
    virtual glm::vec3 ConvertScreenToWorld(glm::vec2 position) = 0;
    
    virtual void FrameSelection() = 0;
};

class OrthographicViewport : public Viewport
{
public:
    OrthographicViewport();
    ~OrthographicViewport();
    
    virtual pb::Camera* GetCamera();
    virtual ViewportType GetType();
    
    virtual glm::vec3 ConvertScreenToWorld(glm::vec2 position);
    
    virtual void FrameSelection();
    
private:
    pb::OrthographicCamera* _Camera;
};

class PerspectiveViewport : public Viewport
{
public:
    PerspectiveViewport();
    ~PerspectiveViewport();
    
    virtual pb::Camera* GetCamera();
    virtual ViewportType GetType();
    
    virtual glm::vec3 ConvertScreenToWorld(glm::vec2 position);
    
    virtual void FrameSelection();
    
private:
    pb::OrbitalCamera* _Camera;
};

class View : public pb::Engine
{
public:
    View(void* platformContext, int argc, const char** argv);
    ~View();
    
public:
    static View* Instance();
    virtual void Initialise();
    virtual void Update(float timeDelta, float gameDelta);
    
    ManipulatorManager* GetManipulatorManager();
    
    void SetDirty();
    
    void Scroll(glm::vec2 offset);
    void Zoom(float delta);
    
public:
    void SetCanvasSize(glm::vec2 size);
    void SetRecord(ProjectRecord* record);
    ProjectRecord* GetRecord();
    
    pb::Scene* GetLevelScene();
    Level* GetLevel();
    Viewport* GetActiveViewport();
    void SetActiveViewport(int index);
    
public:
    sigslot::Signal0<> onRedraw;
    
private:
    void OnDisplayResolutionChanged(glm::vec2 resolution);
    void OnDisplayDensityChanged(float density);
    
private:
    ProjectRecord* _Record;
    
private:
    void OnProjectOpened(Project* project);
    void OnProjectClosed(Project* project);
    
    void OnSelectionChanged();
    
private:
    ManipulatorManager* _ManipulatorManager;
    
    Level* _Level;
    
    Viewport* _ActiveViewport;
    std::vector<Viewport*> _Viewports;
    
    pb::Scene* _LevelScene;
    pb::Viewport* _LevelViewport;

    ViewKeyboardHandler* _KeyboardHandler;
    ViewMouseHandler* _MouseHandler;
    
    Core* _Core;
    int _LaunchArgc;
    const char** _LaunchArgv;
};
