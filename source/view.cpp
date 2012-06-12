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
#include "pixelboost/graphics/renderer/sprite/sprite.h"
#include "pixelboost/graphics/renderer/sprite/spriteRenderer.h"
#include "pixelboost/logic/system/graphics/render/basic.h"
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
#include "view/ui/property/propertyPanel.h"
#include "view/ui/settings/networkWindow.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

class pixeleditor::ViewKeyboardHandler : public pb::KeyboardHandler
{
public:
    ViewKeyboardHandler()
    {
        
    }
    
    ~ViewKeyboardHandler()
    {
        
    }
    
    virtual int GetPriority()
    {
        return 0;
    }
    
    virtual bool OnKeyDown(pb::KeyboardKey key, char character)
    {
        if (key == pb::kKeyboardKeyBackspace || key == pb::kKeyboardKeyDelete)
        {
            Core::Instance()->GetCommandManager()->Exec("delete");
            return true;
        }
        
        return false;
    }
    
    virtual bool OnKeyUp(pb::KeyboardKey key, char character)
    {
        return false;
    }
};

class pixeleditor::ViewMouseHandler : public pb::MouseHandler
{
public:
    ViewMouseHandler()
    {
        
    }
    
    ~ViewMouseHandler()
    {
    }
    
    virtual int GetPriority()
    {
        return 0;
    }
    
    virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
    {
        return false;
    }
    
    virtual bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
    {
        return false;
    }
    
    virtual bool OnMouseMove(glm::vec2 position)
    {
        return false;
    }
    
    virtual bool OnMouseScroll(pb::ModifierKeys modifierKeys, glm::vec2 delta)
    {
        if (modifierKeys & pb::kModifierCtrl)
            View::Instance()->Zoom(delta.y/1200.f);
        else
            View::Instance()->Scroll(delta/10.f);
        return true;
    }
    
    virtual bool OnMouseZoom(glm::vec2 delta)
    {
        View::Instance()->Zoom(delta.x);
        return true;
    }
};

View::View()
    : pb::Game(0)
    , _Record(0)
{
    _KeyboardHandler = new ViewKeyboardHandler();
    _MouseHandler = new ViewMouseHandler();
    
    View::Instance()->GetKeyboardManager()->AddHandler(_KeyboardHandler);
    View::Instance()->GetMouseManager()->AddHandler(_MouseHandler);
}

View::~View()
{
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
    return static_cast<View*>(Game::Instance());
}

void View::Initialise(glm::vec2 size)
{
    float density = 32.f;
    
    pb::GraphicsDevice::Instance()->SetDisplayResolution(size);
    pb::GraphicsDevice::Instance()->SetDisplayDensity(density);
    
    _LevelCamera = new pb::OrthographicCamera();
    _LevelViewport = new pb::Viewport(0, _LevelCamera);
    _LevelScene = new pb::Scene();
    _LevelScene->AddSystem(new pb::BasicRenderSystem());
    _LevelViewport->SetScene(_LevelScene);

    _UiCamera = new pb::OrthographicCamera();
    _UiViewport = new pb::Viewport(0, _UiCamera);
    _UiScene = new pb::Scene();
    _UiScene->AddSystem(new pb::BasicRenderSystem());
    _UiViewport->SetScene(_UiScene);
    _UiViewport->SetDensity(density);
    _UiViewport->SetResolution(size);
    _UiViewport->SetPosition(glm::vec2(0,0));
        
    pb::Renderer::Instance()->AddViewport(_LevelViewport);
    pb::Renderer::Instance()->AddViewport(_UiViewport);
    
    _ManipulatorManager = new ManipulatorManager();
    _ManipulatorManager->AddManipulator(new SelectManipulator());
    _ManipulatorManager->AddManipulator(new MoveManipulator());
    _ManipulatorManager->AddManipulator(new RotateManipulator());
    _ManipulatorManager->AddManipulator(new ScaleManipulator());
    _ManipulatorManager->AddManipulator(new CreateManipulator());
    _ManipulatorManager->SetActiveManipulator("select");
    
    _Level = new Level();
    
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
    Gwen::Controls::MenuItem* deviceAddress = networkMenu->GetMenu()->AddItem("Device Address");
    deviceAddress->onPress.Add(this, &View::OnDeviceAddress);
    
    _GwenRenderable = new pb::GwenRenderable(_GwenCanvas);
    _UiScene->GetSystemByType<pb::RenderSystem>()->AddItem(_GwenRenderable);
    
    _CreateMenu = _Menu->AddItem("Create");
    
    SetCanvasSize(size);
    
    // Register events
    Core::Instance()->GetProject()->projectOpened.Connect(this, &View::OnProjectOpened);
    Core::Instance()->GetProject()->projectClosed.Connect(this, &View::OnProjectClosed);
    Core::Instance()->GetProject()->projectSaved.Connect(this, &View::OnProjectSaved);
    Core::Instance()->GetProject()->projectExported.Connect(this, &View::OnProjectExported);
    Core::Instance()->GetProject()->recordAdded.Connect(this, &View::OnRecordAdded);
    Core::Instance()->GetProject()->recordRemoved.Connect(this, &View::OnRecordRemoved);
    _Level->entityAdded.Connect(this, &View::OnEntityAdded);
    _Level->entityRemoved.Connect(this, &View::OnEntityRemoved);
}

void View::Render()
{
    _Level->Render(0, 1);
    
    _ManipulatorManager->Render(2);
    
    Game::Render();
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

std::string View::GetSpriteFile(const std::string& sprite)
{
    Project* project = Core::Instance()->GetProject();
    
    const Project::ProjectConfig& config = project->GetConfig();
    
    std::fstream file;
    
    for (std::vector<std::string>::const_iterator it = config.imageRoots.begin(); it != config.imageRoots.end(); ++it)
    {
        std::string fileName = *it + sprite + ".png";
        file.open(fileName.c_str());
        
        if (file.is_open())
        {
            file.close();
            return fileName;
        }
    }
    
    return "";
}

void View::LoadSprite(const std::string& sprite)
{
    if (GetSpriteRenderer()->GetSpriteSheet(sprite))
        return;
    
    std::shared_ptr<pb::SpriteSheet> spriteSheet = GetSpriteRenderer()->CreateSpriteSheet(sprite);
    spriteSheet->LoadSingle(GetSpriteFile(sprite));
}

Level* View::GetLevel()
{
    return _Level;
}

pb::OrthographicCamera* View::GetLevelCamera()
{
    return _LevelCamera;
}

void View::SetCanvasSize(glm::vec2 size)
{
    pb::GraphicsDevice::Instance()->SetDisplayResolution(size);

    _GwenCanvas->SetSize(size[0], size[1]);
    _UiViewport->SetResolution(size);
    _UiCamera->Position = glm::vec3(size[0]/2.f, -size[1]/2.f, 0.f)/32.f;
}

void View::SetRecord(Record* record)
{
    _EntityPage->Clear();
    _Entities = new Gwen::Controls::CollapsibleCategory(_EntityPage);
    _Entities->SetText("Entities");
    _EntityPage->Add(_Entities);
    
    _Record = record;
    _Level->SetRecord(_Record);
}

Record* View::GetRecord()
{
    return _Record;
}

void View::OnProjectOpened(Project* project)
{
    _FilePage->Clear();
    _Records = new Gwen::Controls::CollapsibleCategory(_FilePage);
    _Records->SetText("Records");
    _FilePage->Add(_Records);
    
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

void View::OnRecordAdded(Project* project, Record* record)
{
    Gwen::TextObject recordItem(record->GetName());
    Gwen::Controls::Button* button = _Records->Add(recordItem);
    button->UserData.Set("record", record);
    button->onToggleOn.Add(this, &View::OnRecordSelected);
}

void View::OnRecordRemoved(Project* project, Record* record)
{
    
}

void View::OnRecordSelected(Gwen::Controls::Base* item)
{
    Record* record = item->UserData.Get<Record*>("record");
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
    sprintf(args, "-u %d", entity->GetUid());
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
