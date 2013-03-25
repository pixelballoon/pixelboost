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
    class MaterialResource;
    class Model;
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
    pb::OrthographicCamera* GetLevelCamera();
    
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
    
    pb::OrthographicCamera* _LevelCamera;
    pb::Scene* _LevelScene;
    pb::Viewport* _LevelViewport;

    ViewKeyboardHandler* _KeyboardHandler;
    ViewMouseHandler* _MouseHandler;
    
    Core* _Core;
    int _LaunchArgc;
    const char** _LaunchArgv;
};
