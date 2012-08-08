#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/input/components/rectTouch.h"
#include "pixelboost/logic/message/debug/render.h"
#include "pixelboost/input/messages/touch.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/system/debug/render.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

RectTouchComponent::RectTouchComponent(Entity* parent, bool debugRender)
    : Component(parent)
    , _TouchId(0)
    , _DebugRender(debugRender)
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
    return TypeHash("rectTouch");
}


void RectTouchComponent::SetSize(const glm::vec2& size)
{
    _Size = size;
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
   
    _TouchId = touch.GetId();
    
    glm::vec3 position = GetPosition();
    glm::vec2 screenPos = touch.GetViewportPosition();
    glm::vec2 size = _Size/2.f;
    
    _TouchPosition = screenPos;
    
    if (screenPos.x > position.x-size.x && screenPos.x < position.x+size.x &&
        screenPos.y > position.y-size.y && screenPos.y < position.y+size.y)
    {
        TouchMessage message(GetParent(), this, TouchMessage::kTouchTypeDown, screenPos-glm::vec2(position.x, position.y));
        GetScene()->SendMessage(GetParentUid(), message);
        
        return true;
    }
    
    return false;
}

bool RectTouchComponent::OnTouchMove(Touch touch)
{
    if (_TouchId != touch.GetId())
        return false;
    
    glm::vec3 position = GetPosition();
    glm::vec2 screenPos = touch.GetViewportPosition();
    glm::vec2 size = _Size/2.f;
    
    _TouchPosition = screenPos;
    
    TouchMessage message(GetParent(), this, TouchMessage::kTouchTypeMove, screenPos-glm::vec2(position.x, position.y));
    GetScene()->SendMessage(GetParentUid(), message);
    
    return true;
}

bool RectTouchComponent::OnTouchUp(Touch touch)
{
    if (_TouchId != touch.GetId())
        return false;
    
    _TouchId = 0;
    
    glm::vec3 position = GetPosition();
    glm::vec2 screenPos = touch.GetViewportPosition();
    glm::vec2 size = _Size/2.f;
    
    _TouchPosition = screenPos;
    
    TouchMessage message(GetParent(), this, TouchMessage::kTouchTypeUp, screenPos-glm::vec2(position.x, position.y));
    GetScene()->SendMessage(GetParentUid(), message);
    
    return true;
}

void RectTouchComponent::OnDebugRender(pb::Uid sender, const pb::Message& message)
{
    if (_TouchId)
    {
        const pb::DebugRenderMessage& debugRenderMessage = static_cast<const pb::DebugRenderMessage&>(message);
        debugRenderMessage.GetDebugRenderSystem()->AddEllipse(pb::kRenderPassUi, glm::vec3(_TouchPosition, 0.f), glm::vec3(0,0,0), glm::vec2(0.2,0.2));
        debugRenderMessage.GetDebugRenderSystem()->AddEllipse(pb::kRenderPassUi, glm::vec3(_TouchPosition, 0.f), glm::vec3(0,0,0), glm::vec2(3,3));
    }
}
