#include <fstream>

#include "Gwen/Gwen.h"
#include "Gwen/Controls/Layout/Position.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/CollapsibleList.h"
#include "Gwen/Controls/DockBase.h"
#include "Gwen/Controls/ListBox.h"
#include "Gwen/Controls/MenuStrip.h"
#include "Gwen/Controls/TabControl.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/graphics/renderer/gwen/gwenRenderer.h"
#include "pixelboost/graphics/renderer/model/modelRenderer.h"
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/logic/system/graphics/render/bounds.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/scene.h"
#include "pixelboost/misc/gwen/inputHandler.h"

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
#include "view/ui/property/propertyPanel.h"
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
        
        if (event.Key == pb::kKeyboardKeyCharacter && event.Modifier == pb::kModifierKeyControl)
        {
            switch (event.Character)
            {
            case 'e':
                Core::Instance()->GetCommandManager()->Exec("export");
                break;
            case 's':
                Core::Instance()->GetCommandManager()->Exec("save");
                break;
            case 'c':
                Core::Instance()->GetCommandManager()->Exec("copy");
                break;
            case 'v':
            {
                char arguments[32];
                snprintf(arguments, 32, "-r %d", View::Instance()->GetRecord()->GetUid());
                Core::Instance()->GetCommandManager()->Exec("paste", arguments);
                break;
            }
            case '[':
                Core::Instance()->GetCommandManager()->Exec("modifyDepth", "-f");
                break;
            case ']':
                Core::Instance()->GetCommandManager()->Exec("modifyDepth", "-b");
                break;
            }

            return true;
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
    
    pb::GraphicsDevice::Instance()->onDisplayDensityChanged.Connect(sigslot::MakeDelegate(this, &View::OnDisplayDensityChanged));
    pb::GraphicsDevice::Instance()->onDisplayResolutionChanged.Connect(sigslot::MakeDelegate(this, &View::OnDisplayResolutionChanged));
}

View::~View()
{
    pb::GraphicsDevice::Instance()->onDisplayDensityChanged.Disconnect(sigslot::MakeDelegate(this, &View::OnDisplayDensityChanged));
    pb::GraphicsDevice::Instance()->onDisplayResolutionChanged.Disconnect(sigslot::MakeDelegate(this, &View::OnDisplayResolutionChanged));
    
    View::Instance()->GetKeyboardManager()->RemoveHandler(_KeyboardHandler);
    View::Instance()->GetMouseManager()->RemoveHandler(_MouseHandler);
    
    pb::Renderer::Instance()->RemoveViewport(_LevelViewport);
    pb::Renderer::Instance()->RemoveViewport(_UiViewport);
    
    delete _LevelViewport;
    delete _UiViewport;
    
    delete _LevelCamera;
    delete _UiCamera;
    
    delete _LevelScene;
    delete _UiScene;
    
    delete _KeyboardHandler;
    delete _MouseHandler;
    
    delete _GwenInput;
    delete _GwenCanvas;
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

    _UiCamera = new pb::OrthographicCamera();
    _UiViewport = new pb::Viewport(0, _UiCamera);
    _UiScene = new pb::Scene();
    _UiScene->AddSystem(new pb::BoundsRenderSystem());
    _UiViewport->SetScene(_UiScene);
    _UiViewport->SetDensity(32.f);
    _UiViewport->SetPosition(glm::vec2(0,0));
        
    pb::Renderer::Instance()->AddViewport(_LevelViewport);
    pb::Renderer::Instance()->AddViewport(_UiViewport);
    
    _ManipulatorManager = new ManipulatorManager(_LevelScene);
    _ManipulatorManager->AddManipulator(_LevelScene->CreateEntity<SelectManipulator>(0, 0));
    _ManipulatorManager->AddManipulator(_LevelScene->CreateEntity<MoveManipulator>(0, 0));
    _ManipulatorManager->AddManipulator(_LevelScene->CreateEntity<RotateManipulator>(0, 0));
    _ManipulatorManager->AddManipulator(_LevelScene->CreateEntity<ScaleManipulator>(0, 0));
    _ManipulatorManager->AddManipulator(_LevelScene->CreateEntity<CreateManipulator>(0, 0));
    _ManipulatorManager->SetActiveManipulator("select");
    
    _Level = _LevelScene->CreateEntity<Level>(0, 0);
    
    _GwenRenderer = new pb::GwenRenderer();
    _GwenRenderer->Init();
    
    Gwen::Skin::TexturedBase* skin = new Gwen::Skin::TexturedBase(_GwenRenderer);
    skin->Init("skin.png");
    skin->SetDefaultFont(Gwen::Utility::StringToUnicode("helvetica"), 12.f);
    
    _GwenCanvas = new Gwen::Controls::Canvas(skin);
    _GwenCanvas->SetDrawBackground(false);
    
    Gwen::Controls::DockBase* dock = new Gwen::Controls::DockBase(_GwenCanvas);
    dock->Dock(Gwen::Pos::Fill);
    
    _GwenInput = new pb::GwenInputHandler(_GwenCanvas, dock);
    
    _FilePage = new Gwen::Controls::CollapsibleList(dock);
    _EntityPage = new Gwen::Controls::CollapsibleList(dock);
	dock->GetLeft()->GetTabControl()->AddPage("Files", _FilePage);
    dock->GetLeft()->GetTabControl()->AddPage("Entities", _EntityPage);
	dock->GetLeft()->SetWidth(250);
    
    _Output = new Gwen::Controls::ListBox(dock);
    PropertyPanel* _Properties = new PropertyPanel(dock);
    dock->GetBottom()->GetTabControl()->AddPage("Properties", _Properties);
    dock->GetBottom()->GetTabControl()->AddPage("Output", _Output);
    dock->GetBottom()->GetTabControl()->AddPage("Create", new CreatePanel(dock));
	dock->GetBottom()->SetHeight(300);
    
    _Menu = new Gwen::Controls::MenuStrip(dock);
    
    Gwen::Controls::MenuItem* networkMenu = _Menu->AddItem("Network");
    Gwen::Controls::MenuItem* deviceAddress = networkMenu->GetMenu()->AddItem("Device Connection");
    deviceAddress->onPress.Add(this, &View::OnDeviceAddress);
    
    _GwenRenderable = new pb::GwenRenderable(_GwenCanvas);
    _UiScene->GetSystemByType<pb::RenderSystem>()->AddItem(_GwenRenderable);
    
    _CreateMenu = _Menu->AddItem("Create");
    
    Gwen::Controls::MenuItem* recordMenu = _Menu->AddItem("Record");
    Gwen::Controls::MenuItem* createRecord = recordMenu->GetMenu()->AddItem("Create Record");
    createRecord->onPress.Add(this, &View::OnCreateRecord);
    
    Gwen::Controls::MenuItem* manipulatorMenu = _Menu->AddItem("Manipulator");
    Gwen::Controls::MenuItem* moveManipulator = manipulatorMenu->GetMenu()->AddItem("Move Manipulator");
    moveManipulator->onPress.Add(this, &View::OnMoveManipulator);
    Gwen::Controls::MenuItem* rotateManipulator = manipulatorMenu->GetMenu()->AddItem("Rotate Manipulator");
    rotateManipulator->onPress.Add(this, &View::OnRotateManipulator);
    
    SetCanvasSize(pb::GraphicsDevice::Instance()->GetDisplayResolution());
    
    pb::GraphicsDevice::Instance()->SetClearColor(glm::vec4(0.5,0.5,0.5,1));
    
    // Register events
    Core::Instance()->GetProject()->projectOpened.Connect(this, &View::OnProjectOpened);
    Core::Instance()->GetProject()->projectClosed.Connect(this, &View::OnProjectClosed);
    Core::Instance()->GetProject()->projectSaved.Connect(this, &View::OnProjectSaved);
    Core::Instance()->GetProject()->projectExported.Connect(this, &View::OnProjectExported);
    Core::Instance()->GetProject()->recordAdded.Connect(this, &View::OnRecordAdded);
    Core::Instance()->GetProject()->recordRemoved.Connect(this, &View::OnRecordRemoved);
    _Level->entityAdded.Connect(this, &View::OnEntityAdded);
    _Level->entityRemoved.Connect(this, &View::OnEntityRemoved);

    if (_LaunchArgc > 1)
        _Core->GetProject()->Open(_LaunchArgv[1]);
}

void View::Update(float timeDelta, float gameDelta)
{
    Engine::Update(timeDelta, gameDelta);
    
    _LevelScene->Update(timeDelta, gameDelta);
    _UiScene->Update(timeDelta, gameDelta);
    
    _ManipulatorManager->Render(2);
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
    _GwenCanvas->SetSize(size[0], size[1]);
    _LevelViewport->SetResolution(size);
    _UiViewport->SetResolution(size);
    _UiCamera->Position = glm::vec3(size[0]/2.f, -size[1]/2.f, 0.f)/32.f + glm::vec3(0,0,1);
}

void View::SetRecord(ProjectRecord* record)
{
    _EntityPage->Clear();
    _Entities = new Gwen::Controls::CollapsibleCategory(_EntityPage);
    _Entities->SetText("Entities");
    _EntityPage->Add(_Entities);
    
    _Record = record;
    _Level->SetRecord(_Record);
}

ProjectRecord* View::GetRecord()
{
    return _Record;
}

void View::OnProjectOpened(Project* project)
{
    _FilePage->Clear();
    _Records = new Gwen::Controls::CollapsibleCategory(_FilePage);
    _Records->SetText("Records");
    _FilePage->Add(_Records);
    
    int pixelUnit = project->GetConfig().pixelUnit;
    pb::GraphicsDevice::Instance()->SetDisplayDensity(pixelUnit);
    _LevelViewport->SetDensity(pixelUnit);
    
    SetupCreateMenu(project);
}

void View::OnProjectClosed(Project* project)
{
    _CreateMenu->GetMenu()->ClearItems();
    _FilePage->Clear();
    _Level->Clear();
}

void View::OnProjectSaved(Project* project)
{
    
}

void View::OnProjectExported(Project* project)
{
    
}

void View::OnRecordAdded(Project* project, ProjectRecord* record)
{
    Gwen::TextObject recordItem(record->GetName());
    Gwen::Controls::Button* button = _Records->Add(recordItem);
    button->UserData.Set("record", record);
    button->onToggleOn.Add(this, &View::OnRecordSelected);
}

void View::OnRecordRemoved(Project* project, ProjectRecord* record)
{
    
}

void View::OnRecordSelected(Gwen::Controls::Base* item)
{
    ProjectRecord* record = item->UserData.Get<ProjectRecord*>("record");
    SetRecord(record);
}

void View::OnEntityAdded(ViewEntity* entity)
{
    Gwen::TextObject entityItem(entity->GetEntity()->GetName());
    Gwen::Controls::Button* button = _Entities->Add(entityItem);
    button->UserData.Set("entity", entity);
    button->onToggleOn.Add(this, &View::OnEntitySelected);
}

void View::OnEntityRemoved(ViewEntity* entity)
{
    
}

void View::OnEntitySelected(Gwen::Controls::Base* item)
{
    ViewEntity* entity = item->UserData.Get<ViewEntity*>("entity");
    
    char args[256];
    sprintf(args, "-u %d", entity->GetEntityUid());
    Core::Instance()->GetCommandManager()->Exec("select", args);
}

void View::OnEntityCreate(Gwen::Controls::Base* item)
{
    CreateManipulator* createManipulator = static_cast<CreateManipulator*>(View::Instance()->GetManipulatorManager()->SetActiveManipulator("create"));
    
    createManipulator->SetEntityType(item->UserData.Get<std::string>("type"));
}

void View::OnDeviceAddress(Gwen::Controls::Base* item)
{
    new NetworkWindow(_GwenCanvas);
}

void View::OnCreateRecord(Gwen::Controls::Base *item)
{
    new CreateRecordWindow(_GwenCanvas);
}

void View::OnMoveManipulator(Gwen::Controls::Base* item)
{
    new MoveManipulatorWindow(_GwenCanvas);
}

void View::OnRotateManipulator(Gwen::Controls::Base* item)
{
    new RotateManipulatorWindow(_GwenCanvas);
}

void View::OnSelectionChanged()
{
    SetDirty();
}

void View::SetupCreateMenu(Project* project)
{
    Schema* schema = project->GetSchema();
    
    _CreateMenu->GetMenu()->ClearItems();
    for (Schema::EntityMap::const_iterator it = schema->GetEntities().begin(); it != schema->GetEntities().end(); ++it)
    {
        Gwen::Controls::MenuItem* item = _CreateMenu->GetMenu()->AddItem(it->first);
        item->UserData.Set("type", it->first);
        item->onPress.Add(this, &View::OnEntityCreate);
    }
}
