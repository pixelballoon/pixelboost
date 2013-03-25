#include <fstream>

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/font/fontRenderer.h"
#include "pixelboost/graphics/renderer/gwen/gwenRenderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/graphics/resources/materialResource.h"
#include "pixelboost/logic/system/graphics/gui/gui.h"
#include "pixelboost/logic/system/graphics/render/bounds.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/scene.h"

#include "command/manager.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"
#include "view/entity/entity.h"
#include "view/manipulator/create.h"
#include "view/manipulator/manipulator.h"
#include "view/manipulator/move.h"
#include "view/manipulator/rotate.h"
#include "view/manipulator/scale.h"
#include "view/manipulator/select.h"
#include "view/ui/create/createPanel.h"
#include "view/ui/create/createRecord.h"
#include "view/ui/editor.h"
#include "view/ui/property.h"
#include "view/ui/settings/moveManipulator.h"
#include "view/ui/settings/networkWindow.h"
#include "view/ui/settings/rotateManipulator.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

namespace pb
{
    Engine* Engine::Create(void* platformContext, int argc, const char** argv)
    {
        return new View(platformContext, argc, argv);
    }
}

class ViewKeyboardHandler : public pb::KeyboardHandler
{
public:
    ViewKeyboardHandler()
    {
    }
    
    ~ViewKeyboardHandler()
    {
    }
    
    virtual int GetInputHandlerPriority()
    {
        return 0;
    }
    
    virtual bool OnKeyboardEvent(pb::KeyboardEvent event)
    {
        if (event.Type != pb::KeyboardEvent::kKeyboardEventDown)
            return false;
        
        if (event.Key == pb::kKeyboardKeyBackspace || event.Key == pb::kKeyboardKeyDelete)
        {
            Core::Instance()->GetCommandManager()->Exec("delete");
            return true;
        }
        
        if (event.Key == pb::kKeyboardKeyCharacter)
        {
            if (event.Modifier == pb::kModifierKeyControl)
            {
                switch (event.Character)
                {
                    case 'e':
                        Core::Instance()->GetCommandManager()->Exec("export");
                        return true;
                    case 's':
                        Core::Instance()->GetCommandManager()->Exec("save");
                        return true;
                    case 'c':
                        Core::Instance()->GetCommandManager()->Exec("copy");
                        return true;
                    case 'v':
                    {
                        char arguments[32];
                        snprintf(arguments, 32, "-r %d", View::Instance()->GetRecord()->GetUid());
                        Core::Instance()->GetCommandManager()->Exec("paste", arguments);
                        return true;
                    }
                    case '[':
                        Core::Instance()->GetCommandManager()->Exec("modifyDepth", "-f");
                        return true;
                    case ']':
                        Core::Instance()->GetCommandManager()->Exec("modifyDepth", "-b");
                        return true;
                }
            } else if (event.Modifier == pb::kModifierKeyNone) {
                switch (event.Character)
                {
                    case 'f':
                        View::Instance()->GetLevelCamera()->Position = glm::vec3(0,0,500);
                        View::Instance()->GetLevelCamera()->Scale = glm::vec2(1,1);
                        return true;
                }
            }
        }
        
        return false;
    }
};

class ViewMouseHandler : public pb::MouseHandler
{
public:
    ViewMouseHandler()
    {
    }
    
    ~ViewMouseHandler()
    {
    }
    
    virtual int GetInputHandlerPriority()
    {
        return 0;
    }
    
    virtual bool OnMouseEvent(pb::MouseEvent event)
    {
        switch (event.Type)
        {
            case pb::MouseEvent::kMouseEventScroll:
            {
                if (event.Scroll.Modifier & pb::kModifierCtrl)
                {
                    View::Instance()->Zoom(event.Scroll.Delta[1]/1200.f);
                } else
                {
                    View::Instance()->Scroll(glm::vec2(event.Scroll.Delta[0], event.Scroll.Delta[1])/10.f);
                }
                return true;
            }
            case pb::MouseEvent::kMouseEventZoom:
            {
                View::Instance()->Zoom(event.Zoom.Delta[0]);
                return true;
            }
            default:
                return false;
        }
    }
};

View::View(void* platformContext, int argc, const char** argv)
    : pb::Engine(platformContext, argc, argv, false)
    , _LaunchArgc(argc)
    , _LaunchArgv(argv)
    , _Record(0)
{
    _Core = new Core();
    
    _KeyboardHandler = new ViewKeyboardHandler();
    _MouseHandler = new ViewMouseHandler();
    
    View::Instance()->GetKeyboardManager()->AddHandler(_KeyboardHandler);
    View::Instance()->GetMouseManager()->AddHandler(_MouseHandler);
    
    pb::GraphicsDevice::Instance()->onDisplayDensityChanged.Connect(this, &View::OnDisplayDensityChanged);
    pb::GraphicsDevice::Instance()->onDisplayResolutionChanged.Connect(this, &View::OnDisplayResolutionChanged);
}

View::~View()
{
    pb::GraphicsDevice::Instance()->onDisplayDensityChanged.Disconnect(this, &View::OnDisplayDensityChanged);
    pb::GraphicsDevice::Instance()->onDisplayResolutionChanged.Disconnect(this, &View::OnDisplayResolutionChanged);
    
    View::Instance()->GetKeyboardManager()->RemoveHandler(_KeyboardHandler);
    View::Instance()->GetMouseManager()->RemoveHandler(_MouseHandler);
    
    pb::Renderer::Instance()->RemoveViewport(_LevelViewport);
    
    delete _LevelViewport;
    delete _LevelCamera;
    delete _LevelScene;
    
    delete _KeyboardHandler;
    delete _MouseHandler;
}

View* View::Instance()
{
    return static_cast<View*>(Engine::Instance());
}

void View::Initialise()
{
    _LevelCamera = new pb::OrthographicCamera();
    _LevelViewport = new pb::Viewport(0, _LevelCamera);
    _LevelScene = new pb::Scene();
    _LevelScene->AddSystem(new pb::BoundsRenderSystem());
    _LevelViewport->SetScene(_LevelScene);
    _LevelViewport->SetDensity(32.f);
    _LevelViewport->SetPosition(glm::vec2(0,0));
        
    pb::Renderer::Instance()->AddViewport(_LevelViewport);
    
    _ManipulatorManager = new ManipulatorManager(_LevelScene);
    _ManipulatorManager->AddManipulator(_LevelScene->CreateEntity<SelectManipulator>(0, 0));
    _ManipulatorManager->AddManipulator(_LevelScene->CreateEntity<MoveManipulator>(0, 0));
    _ManipulatorManager->AddManipulator(_LevelScene->CreateEntity<RotateManipulator>(0, 0));
    _ManipulatorManager->AddManipulator(_LevelScene->CreateEntity<ScaleManipulator>(0, 0));
    _ManipulatorManager->AddManipulator(_LevelScene->CreateEntity<CreateManipulator>(0, 0));
    _ManipulatorManager->SetActiveManipulator("select");
    
    _Level = _LevelScene->CreateEntity<Level>(0, 0);
    
    _LevelScene->AddSystem(new pb::GuiSystem());
    _LevelScene->CreateEntity<EditorUi>(0, 0);
    
    pb::FontRenderer::Instance()->LoadFont("helvetica", "/fonts/helvetica");
    pb::GraphicsDevice::Instance()->SetClearColor(glm::vec4(43.f/255.f,43.f/255.f,43.f/255.f,1.f));
    
    // Register events
    Core::Instance()->GetProject()->projectOpened.Connect(this, &View::OnProjectOpened);
    Core::Instance()->GetProject()->projectClosed.Connect(this, &View::OnProjectClosed);
    
    if (_LaunchArgc > 1)
        _Core->GetProject()->Open(_LaunchArgv[1]);
}

void View::Update(float timeDelta, float gameDelta)
{
    Engine::Update(timeDelta, gameDelta);
    
    _LevelScene->Update(timeDelta, gameDelta);
    
    _ManipulatorManager->Render(2);
    
    SetCanvasSize(pb::GraphicsDevice::Instance()->GetDisplayResolution());
}

ManipulatorManager* View::GetManipulatorManager()
{
    return _ManipulatorManager;
}

void View::SetDirty()
{
    onRedraw();
}

void View::Scroll(glm::vec2 offset)
{
    _LevelCamera->Position += glm::vec3(offset, 0);
}

void View::Zoom(float delta)
{
    float minZoom = 0.1f;
    float maxZoom = 2.f;
    glm::vec2 scale = _LevelCamera->Scale;
    _LevelCamera->Scale = glm::vec2(glm::max(glm::min(maxZoom, scale[0]+delta), minZoom), glm::max(glm::min(maxZoom, scale[1]+delta), minZoom));
}

pb::Scene* View::GetLevelScene()
{
    return _LevelScene;
}

Level* View::GetLevel()
{
    return _Level;
}

pb::OrthographicCamera* View::GetLevelCamera()
{
    return _LevelCamera;
}

void View::OnDisplayResolutionChanged(glm::vec2 resolution)
{
    SetCanvasSize(resolution);
}

void View::OnDisplayDensityChanged(float density)
{
    
}

void View::SetCanvasSize(glm::vec2 size)
{
    _LevelViewport->SetResolution(size);
}

void View::SetRecord(ProjectRecord* record)
{
    _LevelCamera->Position = glm::vec3(0,0,500);
    _LevelCamera->Scale = glm::vec2(1,1);
    _Record = record;
    _Level->SetRecord(_Record);
}

ProjectRecord* View::GetRecord()
{
    return _Record;
}

void View::OnProjectOpened(Project* project)
{
    int pixelUnit = project->GetConfig().pixelUnit;
    pb::GraphicsDevice::Instance()->SetDisplayDensity(pixelUnit);
    _LevelViewport->SetDensity(pixelUnit);
}

void View::OnProjectClosed(Project* project)
{
    _Level->Clear();
}

void View::OnSelectionChanged()
{
    SetDirty();
}
