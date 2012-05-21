#pragma once

#include <string>

#include "Gwen/Events.h"
#include "sigslot/signal.h"

#include "pixelboost/logic/game.h"
#include "pixelboost/math/maths.h"

#include "manipulator/selection.h"

namespace Gwen
{
    namespace Controls
    {
        class Base;
        class Canvas;
        class CollapsibleCategory;
        class CollapsibleList;
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
class Project;
class Record;

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
    
public:
    void SetCanvasSize(Vec2 size);
    void SetRecord(Record* record);
    
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
        
    Vec2 _CanvasSize;
    
private:
    pb::GwenInputHandler* _GwenInput;
    pb::RenderLayer* _GwenLayer;
    pb::OrthographicCamera* _GwenCamera;
    Gwen::Controls::Canvas* _GwenCanvas;

    Gwen::Controls::CollapsibleList* _FilePage;
    Gwen::Controls::CollapsibleCategory* _Records;    
    Gwen::Controls::CollapsibleList* _EntityPage;
};
    
}
