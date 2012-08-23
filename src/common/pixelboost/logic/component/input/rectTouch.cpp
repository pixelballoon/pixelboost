#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/logic/component/input/rectTouch.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/debug/render.h"
#include "pixelboost/logic/message/input/touch.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

RectTouchComponent::RectTouchComponent(Entity* parent, bool debugRender)
    : Component(parent)
    , _DebugRender(debugRender)
    , _MultiTouch(false)
{
    Game::Instance()->GetTouchManager()->AddHandler(this);
    
    if (_DebugRender)
    {
        GetParent()->RegisterMessageHandler(DebugRenderMessage::GetStaticType(), Entity::MessageHandler(this, &RectTouchComponent::OnDebugRender));
    }
}

RectTouchComponent::~RectTouchComponent()
{
    Game::Instance()->GetTouchManager()->RemoveHandler(this);
    
    if (_DebugRender)
    {
        GetParent()->UnregisterMessageHandler(DebugRenderMessage::GetStaticType(), Entity::MessageHandler(this, &RectTouchComponent::OnDebugRender));
    }
}

Uid RectTouchComponent::GetType()
{
    return GetStaticType();
}

Uid RectTouchComponent::GetStaticType()
{
    return TypeHash("pb::RectTouchComponent");
}


void RectTouchComponent::SetSize(const glm::vec2& size)
{
    _Size = size;
}

void RectTouchComponent::SetMultiTouch(bool multiTouch)
{
    _MultiTouch = multiTouch;
}

glm::vec3 RectTouchComponent::GetPosition()
{
    TransformComponent* transform = GetParent()->GetComponentByType<TransformComponent>();
    
    if (transform)
        return transform->GetPosition();
    
    return glm::vec3(0,0,0);
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
            TouchDownMessage message(GetParent(), this, touch.GetId(), screenPos-glm::vec2(position.x, position.y));
            GetScene()->SendMessage(GetParentUid(), message);
        }
        
        return true;
    }
    
    return false;
}

bool RectTouchComponent::OnTouchMove(Touch touch)
{
    if (!HasTouch(touch))
        return false;
    
    glm::vec3 position = GetPosition();
    glm::vec2 screenPos = touch.GetViewportPosition();
    glm::vec2 size = _Size/2.f;
    
    _Touches[touch.GetId()] = screenPos;
    
    TouchMoveMessage message(GetParent(), this, touch.GetId(), screenPos-glm::vec2(position.x, position.y));
    GetScene()->SendMessage(GetParentUid(), message);
    
    return true;
}

bool RectTouchComponent::OnTouchUp(Touch touch)
{
    if (!HasTouch(touch))
        return false;
    
    RemoveTouch(touch);
    
    glm::vec3 position = GetPosition();
    glm::vec2 screenPos = touch.GetViewportPosition();
    glm::vec2 size = _Size/2.f;
    
    TouchUpMessage message(GetParent(), this, touch.GetId(), screenPos-glm::vec2(position.x, position.y));
    GetScene()->SendMessage(GetParentUid(), message);
    
    return true;
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
