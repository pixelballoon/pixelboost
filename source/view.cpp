#include <fstream>

#include "Gwen/Gwen.h"
#include "Gwen/Controls/Layout/Position.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/CollapsibleList.h"
#include "Gwen/Controls/DockBase.h"
#include "Gwen/Controls/ListBox.h"
#include "Gwen/Controls/TabControl.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/render/common/layer.h"
#include "pixelboost/graphics/render/common/renderer.h"
#include "pixelboost/graphics/render/gwen/gwenRenderer.h"
#include "pixelboost/graphics/render/sprite/sprite.h"
#include "pixelboost/graphics/render/sprite/spriteRenderer.h"
#include "pixelboost/misc/gwen/inputHandler.h"

#include "command/manager.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "view/entity/entity.h"
#include "view/ui/property/propertyPanel.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

View::View()
    : pb::Game(0)
    , _Record(0)
{

}

View::~View()
{
    delete _GwenInput;
    delete _GwenCanvas;
    pb::Game::Instance()->GetRenderer()->RemoveLayer(_GwenLayer);
    delete _GwenLayer;
    delete _GwenCamera;
}

View* View::Instance()
{
    return static_cast<View*>(Game::Instance());
}

void View::Initialise(Vec2 size)
{
    _LevelCamera = new pb::OrthographicCamera();
    _LevelLayer = new pb::RenderLayer(100, _LevelCamera);
    
    _Level = new Level();
    
    _GwenCamera = new pb::OrthographicCamera();
    _GwenLayer = new pb::RenderLayer(500, _GwenCamera);
    
    pb::Game::Instance()->GetRenderer()->AddLayer(_LevelLayer);
    pb::Game::Instance()->GetRenderer()->AddLayer(_GwenLayer);

    pb::GwenRenderer* renderer = new pb::GwenRenderer(_GwenLayer);
    renderer->Init();
    
    Gwen::Skin::TexturedBase* skin = new Gwen::Skin::TexturedBase(renderer);
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
    
    Gwen::Controls::ListBox* _Output = new Gwen::Controls::ListBox(dock);
    PropertyPanel* _Properties = new PropertyPanel(dock);
    dock->GetBottom()->GetTabControl()->AddPage("Properties", _Properties);
    dock->GetBottom()->GetTabControl()->AddPage("Output", _Output);
	dock->GetBottom()->SetHeight(300);
    
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

Vec2 View::GetScreenResolution()
{
    return _CanvasSize;
}

void View::Render()
{
    _GwenCanvas->RenderCanvas();
    
    _Level->Render(_LevelLayer);
    
    Game::Render();
}

void View::SetDirty()
{
    onRedraw();
}

void View::Scroll(Vec2 offset)
{
    _LevelCamera->Position += offset;
}

void View::Zoom(float delta)
{
    float minZoom = 0.1f;
    float maxZoom = 2.f;
    Vec2 scale = _LevelCamera->Scale;
    _LevelCamera->Scale = Vec2(Max(Min(maxZoom, scale[0]+delta), minZoom), Max(Min(maxZoom, scale[1]+delta), minZoom));
}

void View::LoadSprite(const std::string& sprite)
{
    if (GetSpriteRenderer()->GetSpriteSheet(sprite))
        return;
    
    std::shared_ptr<pb::SpriteSheet> spriteSheet = GetSpriteRenderer()->CreateSpriteSheet(sprite);
    spriteSheet->LoadSingle(GetSpriteFile(sprite));
}

pb::OrthographicCamera* View::GetLevelCamera()
{
    return _LevelCamera;
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

void View::SetCanvasSize(Vec2 size)
{
    _CanvasSize = size;
    _GwenCanvas->SetSize(size[0], size[1]);
    _GwenCamera->Position = Vec2(size[0]/2.f, -size[1]/2.f)/32.f;
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
}

void View::OnProjectClosed(Project* project)
{
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

void View::OnSelectionChanged()
{
    SetDirty();
}
