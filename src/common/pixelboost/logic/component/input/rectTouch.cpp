#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/logic/component/input/rectTouch.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/debug/render.h"
#include "pixelboost/logic/message/input/touch.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

PB_DEFINE_COMPONENT(pb::RectTouchComponent)

RectTouchComponent::RectTouchComponent(Entity* parent, bool debugRender)
    : Component(parent)
    , _CaptureEvents(false)
    , _DebugRender(debugRender)
    , _MultiTouch(false)
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

glm::vec3 RectTouchComponent::GetPosition()
{
    glm::vec4 position;
    
    TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();
    
    if (transform)
    {
        glm::mat4x4 transformMatrix = transform->GetMatrix() * _LocalTransform;
        position = transformMatrix * glm::vec4(0,0,0,1);
    } else {
        position = _LocalTransform * glm::vec4(0,0,0,1);
    }
    
    return glm::vec3(position.x, position.y, position.z);
}

int RectTouchComponent::GetInputHandlerPriority()
{
    return 0;
}

bool RectTouchComponent::OnTouchDown(Touch touch)
{
    if (touch.GetViewport()->GetScene() != GetScene())
        return false;
   
    glm::vec3 position = GetPosition();
    glm::vec2 screenPos = touch.GetViewportPosition();
    glm::vec2 size = _Size/2.f;
    
    if (screenPos.x > position.x-size.x && screenPos.x < position.x+size.x &&
        screenPos.y > position.y-size.y && screenPos.y < position.y+size.y)
    {
        if (AddTouch(touch, screenPos))
        {
            TouchDownMessage message(GetEntity(), this, touch.GetId(), screenPos-glm::vec2(position.x, position.y));
            GetScene()->SendMessage(GetEntityUid(), message);
        }
        
        return _CaptureEvents;
    }
    
    return false;
}

bool RectTouchComponent::OnTouchMove(Touch touch)
{
    if (!HasTouch(touch))
        return false;
    
    glm::vec3 position = GetPosition();
    glm::vec2 screenPos = touch.GetViewportPosition();
    
    _Touches[touch.GetId()] = screenPos;
    
    TouchMoveMessage message(GetEntity(), this, touch.GetId(), screenPos-glm::vec2(position.x, position.y));
    GetScene()->SendMessage(GetEntityUid(), message);
    
    return _CaptureEvents;
}

bool RectTouchComponent::OnTouchUp(Touch touch)
{
    if (!HasTouch(touch))
        return false;
    
    RemoveTouch(touch);
    
    glm::vec3 position = GetPosition();
    glm::vec2 screenPos = touch.GetViewportPosition();
    
    TouchUpMessage message(GetEntity(), this, touch.GetId(), screenPos-glm::vec2(position.x, position.y));
    GetScene()->SendMessage(GetEntityUid(), message);
    
    return _CaptureEvents;
}

void RectTouchComponent::OnDebugRender(const pb::Message& message)
{
    for (std::map<int, glm::vec2>::iterator it = _Touches.begin(); it != _Touches.end(); ++it)
    {
        const pb::DebugRenderMessage& debugRenderMessage = static_cast<const pb::DebugRenderMessage&>(message);
        debugRenderMessage.GetDebugRenderSystem()->AddEllipse(pb::kRenderPassUi, 16, glm::vec3(it->second, 0.f), glm::vec3(0,0,0), glm::vec2(0.2,0.2));
        debugRenderMessage.GetDebugRenderSystem()->AddEllipse(pb::kRenderPassUi, 16, glm::vec3(it->second, 0.f), glm::vec3(0,0,0), glm::vec2(3,3));
    }
}

bool RectTouchComponent::AddTouch(Touch touch, glm::vec2 position)
{
    if (_Touches.size() && !_MultiTouch)
        return false;
    
    _Touches[touch.GetId()] = position;
    
    return true;
}

void RectTouchComponent::RemoveTouch(Touch touch)
{
    std::map<int, glm::vec2>::iterator it = _Touches.find(touch.GetId());
    if (it != _Touches.end())
        _Touches.erase(it);
}

bool RectTouchComponent::HasTouch(Touch touch)
{
    return _Touches.find(touch.GetId()) != _Touches.end();
}
