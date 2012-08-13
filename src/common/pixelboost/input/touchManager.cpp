#ifndef PIXELBOOST_DISABLE_INPUT

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/input/touchManager.h"

using namespace pb;

Touch::Touch()
    : _IsValid(false)
{
    
}

Touch::Touch(int touchId, Viewport* viewport, glm::vec2 position)
    : _IsValid(true)
    , _Id(touchId)
    , _Viewport(viewport)
    , _Position(position)
{

}

bool Touch::IsValid()
{
    return _IsValid;
}

int Touch::GetId()
{
    return _Id;
}

Viewport* Touch::GetViewport()
{
    return _Viewport;
}

glm::vec2 Touch::GetScreenPosition()
{
    return _Position;
}

glm::vec2 Touch::GetViewportPosition()
{
    if (!_Viewport)
        return glm::vec2(0,0);
    
    glm::vec2 position = _Position;
    
    position.x = position.x - GraphicsDevice::Instance()->GetDisplayResolution().x/2.f;
    position.y = GraphicsDevice::Instance()->GetDisplayResolution().y/2.f - position[1];
    
    position /= GraphicsDevice::Instance()->GetDisplayDensity();
    
    if (_Viewport->GetUiCamera()->GetType() == Camera::kCameraOrthographic)
        position /= static_cast<OrthographicCamera*>(_Viewport->GetUiCamera())->Scale;
    
    return position;
}

bool Touch::operator==(Touch const& b)
{
    return (IsValid() && b._IsValid && _Id == b._Id);
}

bool Touch::operator!=(Touch const& b)
{
    return !(*this == b);
}

TouchHandler::TouchHandler()
{
    
}

TouchHandler::~TouchHandler()
{
    
}

bool TouchHandler::OnTouchDown(Touch touch)
{
    return false;
}

bool TouchHandler::OnTouchUp(Touch touch)
{
    return false;
}

bool TouchHandler::OnTouchMove(Touch touch)
{
    return false;
}

TouchManager::TouchManager()
{
    
}

TouchManager::~TouchManager()
{

}

void TouchManager::OnTouchDown(int touchId, glm::vec2 position)
{
    UpdateHandlers();
    
    for (HandlerList::iterator handlerIt = _Handlers.begin(); handlerIt != _Handlers.end(); ++handlerIt)
    {
        const Renderer::ViewportList& viewports = Renderer::Instance()->GetViewports();
        for (Renderer::ViewportList::const_iterator viewportIt = viewports.begin(); viewportIt != viewports.end(); ++viewportIt)
        {
            Touch touch(touchId, *viewportIt, position);
            if (static_cast<TouchHandler*>(*handlerIt)->OnTouchDown(touch))
                return;
        }
    }
}

void TouchManager::OnTouchUp(int touchId, glm::vec2 position)
{
    UpdateHandlers();
    
    for (HandlerList::iterator handlerIt = _Handlers.begin(); handlerIt != _Handlers.end(); ++handlerIt)
    {
        const Renderer::ViewportList& viewports = Renderer::Instance()->GetViewports();
        for (Renderer::ViewportList::const_iterator viewportIt = viewports.begin(); viewportIt != viewports.end(); ++viewportIt)
        {
            Touch touch(touchId, *viewportIt, position);
            static_cast<TouchHandler*>(*handlerIt)->OnTouchUp(touch);
        }
    }
}

void TouchManager::OnTouchMove(int touchId, glm::vec2 position)
{
    UpdateHandlers();
    
    for (HandlerList::iterator handlerIt = _Handlers.begin(); handlerIt != _Handlers.end(); ++handlerIt)
    {
        const Renderer::ViewportList& viewports = Renderer::Instance()->GetViewports();
        for (Renderer::ViewportList::const_iterator viewportIt = viewports.begin(); viewportIt != viewports.end(); ++viewportIt)
        {
            Touch touch(touchId, *viewportIt, position);
            if (static_cast<TouchHandler*>(*handlerIt)->OnTouchMove(touch))
                return;
        }
    }
}

#endif
