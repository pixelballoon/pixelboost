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

void View::Initialise()
{
    _GwenCamera = new pb::OrthographicCamera();
    _GwenLayer = new pb::RenderLayer(500, _GwenCamera);
    
    pb::Game::Instance()->GetRenderer()->AddLayer(_GwenLayer);

    pb::GwenRenderer* renderer = new pb::GwenRenderer(_GwenLayer);
    renderer->Init();
    
    Gwen::Skin::TexturedBase* skin = new Gwen::Skin::TexturedBase(renderer);
    skin->Init("skin.png");
    
    _GwenCanvas = new Gwen::Controls::Canvas(skin);
    _GwenCanvas->SetSize(1200, 600);
    _GwenCanvas->SetDrawBackground(false);
    
    _GwenInput = new pb::GwenInputHandler(_GwenCanvas);
    
    SetCanvasSize(Vec2(1200, 600));
    
    Gwen::Controls::DockBase* dock = new Gwen::Controls::DockBase(_GwenCanvas);
    dock->Dock(Gwen::Pos::Fill);
    
    Gwen::Controls::CollapsibleList* filePage = new Gwen::Controls::CollapsibleList(dock);
    Gwen::Controls::CollapsibleList* entityPage = new Gwen::Controls::CollapsibleList(dock);
	dock->GetLeft()->GetTabControl()->AddPage("Files", filePage);
    dock->GetLeft()->GetTabControl()->AddPage("Entities", entityPage);
	dock->GetLeft()->SetWidth(250);
    
    Gwen::Controls::ListBox* _Output = new Gwen::Controls::ListBox(dock);
    dock->GetBottom()->GetTabControl()->AddPage("Output", _Output);
	dock->GetBottom()->SetHeight(200);
    
    Gwen::Controls::Layout::Center* center = new Gwen::Controls::Layout::Center(dock);
	center->Dock(Gwen::Pos::Fill);
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
