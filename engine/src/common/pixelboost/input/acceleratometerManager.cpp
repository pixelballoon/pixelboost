#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/graphics/renderer/common/renderer.h"
#include "pixelboost/input/acceleratometerManager.h"

using namespace pb;

AccelerometerHandler::AccelerometerHandler()
{
    
}

AccelerometerHandler::~AccelerometerHandler()
{
    
}

bool AccelerometerHandler::OnAccelerometer(glm::vec3 attitude, glm::vec3 rotationRate, glm::vec3 gravity, glm::vec3 userAcceleration)
{
    return false;
}

AccelerometerManager::AccelerometerManager()
{
    
}

AccelerometerManager::~AccelerometerManager()
{

}

void AccelerometerManager::OnAccelerometer(glm::vec3 attitude, glm::vec3 rotationRate, glm::vec3 gravity, glm::vec3 userAcceleration)
{
    UpdateHandlers();
    
    for (HandlerList::iterator handlerIt = _Handlers.begin(); handlerIt != _Handlers.end(); ++handlerIt)
    {
        const Renderer::ViewportList& viewports = Renderer::Instance()->GetViewports();
        for (Renderer::ViewportList::const_iterator viewportIt = viewports.begin(); viewportIt != viewports.end(); ++viewportIt)
        {
            if (dynamic_cast<AccelerometerHandler*>(*handlerIt)->OnAccelerometer(attitude, rotationRate, gravity, userAcceleration))
                return;
        }
    }
}
