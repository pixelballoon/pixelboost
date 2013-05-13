#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/debug/log.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/graphics/device/device.h"
#include "pixelboost/logic/component/input/rectTouch.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/debug/render.h"
#include "pixelboost/logic/message/input/touch.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::RectTouchComponent)

RectTouchComponent::TouchInfo::TouchInfo()
{
    
}

RectTouchComponent::TouchInfo::TouchInfo(glm::vec2 startScreenPosition, glm::vec2 startRelativePosition)
{
    StartScreenPosition = CurrentScreenPosition = startScreenPosition;
    StartRelativePosition = CurrentRelativePosition = startRelativePosition;
}

RectTouchComponent::RectTouchComponent(Entity* parent, bool debugRender)
    : Component(parent)
    , _CaptureEvents(false)
    , _DebugRender(debugRender)
    , _MultiTouch(false)
    , _IsUiComponent(false)
    , _PressDistance(1.f)
{
    Engine::Instance()->GetTouchManager()->AddHandler(this);
    
    if (_DebugRender)
    {
        GetEntity()->RegisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &RectTouchComponent::OnDebugRender));
    }
}

RectTouchComponent::~RectTouchComponent()
{
    Engine::Instance()->GetTouchManager()->RemoveHandler(this);
    
    if (_DebugRender)
    {
        GetEntity()->UnregisterMessageHandler<DebugRenderMessage>(MessageHandler(this, &RectTouchComponent::OnDebugRender));
    }
}

void RectTouchComponent::SetLocalTransform(const glm::mat4x4& localTransform)
{
    _LocalTransform = localTransform;
}

const glm::mat4x4& RectTouchComponent::GetLocalTransform()
{
    return _LocalTransform;
}

void RectTouchComponent::SetSize(const glm::vec2& size)
{
    _Size = size;
}

void RectTouchComponent::SetCaptureEvents(bool captureEvents)
{
    _CaptureEvents = captureEvents;
}

void RectTouchComponent::SetMultiTouch(bool multiTouch)
{
    _MultiTouch = multiTouch;
}

void RectTouchComponent::SetIsUiComponent(bool isUiComponent)
{
    _IsUiComponent = isUiComponent;
}

Ray RectTouchComponent::GetTouchRay(TouchEvent touch)
{   
    pb::Camera* camera = 0;
    
    if (_IsUiComponent)
    {
        camera = touch.GetViewport()->GetUiCamera();
    } else {
        camera = touch.GetViewport()->GetSceneCamera();
    }
    
    glm::mat4x4 transformMatrix;
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();
    
    if (transform)
    {
        transformMatrix = transform->GetMatrix() * _LocalTransform;
    } else {
        transformMatrix = _LocalTransform;
    }

    glm::vec3 touchPosition(touch.GetScreenPosition().x, touch.GetViewport()->GetResolution().y - touch.GetScreenPosition().y, 0);
    
    glm::vec3 start = glm::unProject(touchPosition, camera->ViewMatrix * transformMatrix, camera->ProjectionMatrix, touch.GetViewport()->GetNativeRegion());
    glm::vec3 end = glm::unProject(touchPosition + glm::vec3(0,0,1), camera->ViewMatrix * transformMatrix, camera->ProjectionMatrix, touch.GetViewport()->GetNativeRegion());
    
    return Ray(start, glm::normalize(end-start));
}

Plane RectTouchComponent::GetPlane()
{
    glm::vec4 position;
    glm::vec4 normal;
    
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();
    
    if (transform)
    {
        glm::mat4x4 transformMatrix = transform->GetMatrix() * _LocalTransform;
        position = transformMatrix * glm::vec4(0,0,0,1);
        normal = transformMatrix * glm::vec4(0,0,1,0);
    } else {
        position = _LocalTransform * glm::vec4(0,0,0,1);
        normal = _LocalTransform * glm::vec4(0,0,1,0);
    }
    
    return Plane(glm::vec3(position.x, position.y, position.z), glm::vec3(normal.x, normal.y, normal.z));
}

int RectTouchComponent::GetInputHandlerPriority()
{
    return 0;
}

bool RectTouchComponent::OnTouchDown(TouchEvent touch)
{
    if (touch.GetViewport()->GetScene() != GetScene())
        return false;
    
    Ray ray = GetTouchRay(touch);
    Plane plane = GetPlane();
    
    auto result = plane.GetIntersection(ray);
    
    if (result.first)
    {
        glm::vec2 size = _Size/2.f;
        
        if (result.second.x > -size.x && result.second.x < size.x &&
            result.second.y > -size.y && result.second.y < size.y)
        {
            glm::vec2 touchPosition(result.second.x, result.second.y);
            if (AddTouch(touch, touchPosition))
            {
                TouchDownMessage message(GetEntity(), this, touch.GetId(), touchPosition);
                GetScene()->SendMessage(GetEntityUid(), message);
            }
            
            return _CaptureEvents;
        }
    }

    return false;
}

bool RectTouchComponent::OnTouchMove(TouchEvent touch)
{
    if (!HasTouch(touch))
        return false;
    
    Ray ray = GetTouchRay(touch);
    Plane plane = GetPlane();
    
    auto result = plane.GetIntersection(ray);
    
    if (result.first)
    {
        glm::vec2 touchPosition(result.second.x, result.second.y);
        
        _Touches[touch.GetId()].CurrentRelativePosition = touchPosition;
        
        TouchMoveMessage message(GetEntity(), this, touch.GetId(), touchPosition);
        GetScene()->SendMessage(GetEntityUid(), message);
    }
    
    return _CaptureEvents;
}

bool RectTouchComponent::OnTouchUp(TouchEvent touch)
{
    if (!HasTouch(touch))
        return false;
    
    Ray ray = GetTouchRay(touch);
    Plane plane = GetPlane();
    
    auto result = plane.GetIntersection(ray);
    
    glm::vec2 touchPosition(result.second.x, result.second.y);
    
    if (result.first)
    {
        glm::vec2 size = _Size/2.f;
        
        if (result.second.x > -size.x && result.second.x < size.x &&
            result.second.y > -size.y && result.second.y < size.y)
        {
            glm::vec2 startPos = _Touches[touch.GetId()].StartScreenPosition;
            if (glm::distance(touch.GetScreenPosition(), startPos) < (_PressDistance*pb::GraphicsDevice::Instance()->GetDisplayDensity()))
            {
                TouchPressedMessage message(GetEntity(), this, touch.GetId(), touchPosition);
                GetScene()->SendMessage(GetEntityUid(), message);
            }
        }
    }
    
    RemoveTouch(touch);
    
    TouchUpMessage message(GetEntity(), this, touch.GetId(), touchPosition);
    GetScene()->SendMessage(GetEntityUid(), message);
    
    return _CaptureEvents;
}

void RectTouchComponent::OnDebugRender(const pb::Message& message)
{
    for (const auto& touch : _Touches)
    {
        auto debugRenderMessage = message.As<DebugRenderMessage>();
        debugRenderMessage.GetDebugRenderSystem()->AddEllipse(pb::kRenderPassUi, 16, glm::vec3(touch.second.CurrentScreenPosition, 0.f), glm::vec3(0,0,0), glm::vec2(0.2,0.2));
        debugRenderMessage.GetDebugRenderSystem()->AddEllipse(pb::kRenderPassUi, 16, glm::vec3(touch.second.CurrentScreenPosition, 0.f), glm::vec3(0,0,0), glm::vec2(3,3));
    }
}

bool RectTouchComponent::AddTouch(TouchEvent touch, glm::vec2 relativePosition)
{
    if (_Touches.size() && !_MultiTouch)
        return false;
    
    _Touches[touch.GetId()] = TouchInfo(touch.GetScreenPosition(), relativePosition);
    
    return true;
}

void RectTouchComponent::RemoveTouch(TouchEvent touch)
{
    auto it = _Touches.find(touch.GetId());
    if (it != _Touches.end())
        _Touches.erase(it);
}

bool RectTouchComponent::HasTouch(TouchEvent touch)
{
    return _Touches.find(touch.GetId()) != _Touches.end();
}
