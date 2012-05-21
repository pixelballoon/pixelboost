#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/render/common/layer.h"
#include "pixelboost/graphics/render/common/renderer.h"
#include "pixelboost/graphics/render/gwen/gwenRenderer.h"
#include "pixelboost/misc/gwen/inputHandler.h"

#include "Gwen/Gwen.h"
#include "Gwen/Controls/Layout/Position.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/CollapsibleList.h"
#include "Gwen/Controls/DockBase.h"
#include "Gwen/Controls/ListBox.h"
#include "Gwen/Controls/TabControl.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"

#include "project/project.h"
#include "project/record.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

View::View()
    : pb::Game(0)
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
    _GwenCamera = new pb::OrthographicCamera();
    _GwenLayer = new pb::RenderLayer(500, _GwenCamera);
    
    pb::Game::Instance()->GetRenderer()->AddLayer(_GwenLayer);

    pb::GwenRenderer* renderer = new pb::GwenRenderer(_GwenLayer);
    renderer->Init();
    
    Gwen::Skin::TexturedBase* skin = new Gwen::Skin::TexturedBase(renderer);
    skin->Init("skin.png");
    
    _GwenCanvas = new Gwen::Controls::Canvas(skin);
    _GwenCanvas->SetDrawBackground(false);
    
    _GwenInput = new pb::GwenInputHandler(_GwenCanvas);
    
    Gwen::Controls::DockBase* dock = new Gwen::Controls::DockBase(_GwenCanvas);
    dock->Dock(Gwen::Pos::Fill);
    
    _FilePage = new Gwen::Controls::CollapsibleList(dock);
    _EntityPage = new Gwen::Controls::CollapsibleList(dock);
	dock->GetLeft()->GetTabControl()->AddPage("Files", _FilePage);
    dock->GetLeft()->GetTabControl()->AddPage("Entities", _EntityPage);
	dock->GetLeft()->SetWidth(250);
    
    Gwen::Controls::ListBox* _Output = new Gwen::Controls::ListBox(dock);
    Gwen::Controls::ListBox* _Properties = new Gwen::Controls::ListBox(dock);
    dock->GetBottom()->GetTabControl()->AddPage("Properties", _Properties);
    dock->GetBottom()->GetTabControl()->AddPage("Output", _Output);
	dock->GetBottom()->SetHeight(300);
    
    Gwen::Controls::Layout::Center* center = new Gwen::Controls::Layout::Center(dock);
	center->Dock(Gwen::Pos::Fill);
    
    SetCanvasSize(size);
    
    // Register events
    Core::Instance()->GetProject()->projectOpened.Connect(this, &View::OnProjectOpened);
    Core::Instance()->GetProject()->projectClosed.Connect(this, &View::OnProjectClosed);
    Core::Instance()->GetProject()->projectSaved.Connect(this, &View::OnProjectSaved);
    Core::Instance()->GetProject()->projectExported.Connect(this, &View::OnProjectExported);
    Core::Instance()->GetProject()->recordAdded.Connect(this, &View::OnRecordAdded);
    Core::Instance()->GetProject()->recordRemoved.Connect(this, &View::OnRecordRemoved);
}

Vec2 View::GetScreenResolution()
{
    return _CanvasSize;
}

void View::Render()
{
    _GwenCanvas->RenderCanvas();
    
    Game::Render();
}

void View::SetCanvasSize(Vec2 size)
{
    _CanvasSize = size;
    _GwenCanvas->SetSize(size[0], size[1]);
    _GwenCamera->Position = Vec2(size[0]/2.f, -size[1]/2.f)/32.f;
}

void View::SetRecord(Record* record)
{
    _Record = record;
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
