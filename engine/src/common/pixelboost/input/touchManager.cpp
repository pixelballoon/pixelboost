#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/input/touchManager.h"

using namespace pb;

TouchEvent::TouchEvent()
    : _IsValid(false)
{
    
}

TouchEvent::TouchEvent(int touchId, Viewport* viewport, glm::vec2 position)
    : _IsValid(true)
    , _Id(touchId)
    , _Viewport(viewport)
    , _Position(position)
{

}

bool TouchEvent::IsValid()
{
    return _IsValid;
}

int TouchEvent::GetId()
{
    return _Id;
}

Viewport* TouchEvent::GetViewport()
{
    return _Viewport;
}

glm::vec2 TouchEvent::GetScreenPosition()
{
    return _Position;
}

glm::vec2 TouchEvent::GetViewportPosition()
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

bool TouchEvent::operator==(TouchEvent const& b)
{
    return (IsValid() && b._IsValid && _Id == b._Id);
}

bool TouchEvent::operator!=(TouchEvent const& b)
{
    return !(*this == b);
}

TouchHandler::TouchHandler()
{
    
}

TouchHandler::~TouchHandler()
{
    
}

bool TouchHandler::OnTouchDown(TouchEvent touch)
{
    return false;
}

bool TouchHandler::OnTouchUp(TouchEvent touch)
{
    return false;
}

bool TouchHandler::OnTouchMove(TouchEvent touch)
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
            TouchEvent touch(touchId, *viewportIt, position);
            if (dynamic_cast<TouchHandler*>(*handlerIt)->OnTouchDown(touch))
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
            TouchEvent touch(touchId, *viewportIt, position);
            dynamic_cast<TouchHandler*>(*handlerIt)->OnTouchUp(touch);
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
            TouchEvent touch(touchId, *viewportIt, position);
            if (dynamic_cast<TouchHandler*>(*handlerIt)->OnTouchMove(touch))
                return;
        }
    }
}
