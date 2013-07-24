#include "pixelboost/framework/screen.h"
#include "pixelboost/input/touchManager.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/renderer/common/renderer.h"

using namespace pb;

Screen::Screen()
    : _Active(false)
{
    
}

Screen::~Screen()
{
    for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
    {
        delete *it;
    }
}

void Screen::Update(float timeDelta, float gameDelta)
{
    
}

bool Screen::IsActive()
{
    return _Active;
}

void Screen::SetActive(bool active)
{
    _Active = active;
    
    if (active)
    {
        for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
        {
            Renderer::Instance()->AddViewport(*it);
        }
    } else {
        for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
        {
            Renderer::Instance()->RemoveViewport(*it);
        }
    }
}
    
void Screen::AddViewport(Viewport* viewport)
{
    _Viewports.push_back(viewport);
    
    if (_Active)
    {
        Renderer::Instance()->AddViewport(viewport);
    }
}
    
void Screen::DestroyViewport(Viewport* viewport)
{
    if (_Active)
    {
        Renderer::Instance()->RemoveViewport(viewport);
    }
    
    for (ViewportList::iterator it = _Viewports.begin(); it != _Viewports.end(); ++it)
    {
        if (*it == viewport)
        {
            delete viewport;
            _Viewports.erase(it);
            return;
        }
    }
}
    
const Screen::ViewportList& Screen::GetViewports()
{
    return _Viewports;
}
