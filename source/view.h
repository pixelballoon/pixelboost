#pragma once

#include <string>

#include "glm/glm.hpp"
#include "Gwen/Events.h"
#include "sigslot/signal.h"

#include "pixelboost/logic/game.h"
#include "pixelboost/math/maths.h"

#include "core/selection.h"

namespace Gwen
{
    namespace Controls
    {
        class Base;
        class Canvas;
        class CollapsibleCategory;
        class CollapsibleList;
        class ListBox;
        class MenuItem;
        class MenuStrip;
    }
}

namespace pb
{
    class GwenInputHandler;
    class OrthographicCamera;
    class RenderLayer;
}

namespace pixeleditor
{

class CommandManager;
class HttpInterface;
class Level;
class ManipulatorManager;
class Project;
class Record;
class ViewEntity;
class ViewKeyboardHandler;
class ViewMouseHandler;

class View : public pb::Game, public Gwen::Event::Handler
{
public:
    View();
    ~View();
    
public:
    static View* Instance();
    virtual void Initialise(Vec2 size);
    virtual Vec2 GetScreenResolution();
    virtual void Render();
    
    ManipulatorManager* GetManipulatorManager();
    
    void SetDirty();
    
    void Scroll(glm::vec2 offset);
    void Zoom(float delta);
    
public:
    void SetCanvasSize(Vec2 size);
    void SetRecord(Record* record);
    Record* GetRecord();
    
    std::string GetSpriteFile(const std::string& sprite);
    void LoadSprite(const std::string& sprite);
    
    Level* GetLevel();
    pb::OrthographicCamera* GetLevelCamera();
    
public:
    sigslot::Signal0<> onRedraw;
    
private:
    Record* _Record;
    
private:
    void OnProjectOpened(Project* project);
    void OnProjectClosed(Project* project);
    void OnProjectSaved(Project* project);
    void OnProjectExported(Project* project);
    
    void OnRecordAdded(Project* project, Record* record);
    void OnRecordRemoved(Project* project, Record* record);
    
    void OnRecordSelected(Gwen::Controls::Base* item);
    
    void OnEntityAdded(ViewEntity* entity);
    void OnEntityRemoved(ViewEntity* entity);
    
    void OnEntitySelected(Gwen::Controls::Base* item);
    void OnEntityCreate(Gwen::Controls::Base* item);
    
    void OnSelectionChanged();
    
    void SetupCreateMenu(Project* project);
    
private:
    ManipulatorManager* _ManipulatorManager;
    
    Level* _Level;
    pb::RenderLayer* _LevelLayer;
    pb::OrthographicCamera* _LevelCamera;
    
    pb::GwenInputHandler* _GwenInput;
    pb::RenderLayer* _GwenLayer;
    pb::OrthographicCamera* _GwenCamera;
    Gwen::Controls::Canvas* _GwenCanvas;
    Vec2 _CanvasSize;

    Gwen::Controls::MenuStrip* _Menu;
    Gwen::Controls::MenuItem* _CreateMenu;
    Gwen::Controls::CollapsibleList* _FilePage;
    Gwen::Controls::CollapsibleCategory* _Records;    
    Gwen::Controls::CollapsibleList* _EntityPage;
    Gwen::Controls::CollapsibleCategory* _Entities;
    Gwen::Controls::ListBox* _Output;
    
    ViewKeyboardHandler* _KeyboardHandler;
    ViewMouseHandler* _MouseHandler;
};
    
}
