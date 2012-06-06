#include "pixelboost/framework/game.h"
#include "pixelboost/input/components/rectTouch.h"
#include "pixelboost/input/messages/touch.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/entity.h"

using namespace pb;

RectTouchComponent::RectTouchComponent(Entity* parent)
    : Component(parent)
{
    Game::Instance()->GetTouchManager()->AddTouchHandler(this);
}

RectTouchComponent::~RectTouchComponent()
{
    Game::Instance()->GetTouchManager()->RemoveTouchHandler(this);
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

void RectTouchComponent::OnTouchBegin(Touch* touch)
{
    glm::vec3 position = GetPosition();
    glm::vec2 screenPos = touch->GetScreenPosition();
    glm::vec2 size = _Size/2.f;
    
    if (screenPos.x > position.x-size.x && screenPos.x < position.x+size.x &&
        screenPos.y > position.y-size.y && screenPos.y < position.y+size.y)
    {
        TouchMessage message(GetParent(), this);
        GetParent()->SendMessage(message);
    }
}

void RectTouchComponent::OnTouchUpdate(Touch* touch)
{
    
}

void RectTouchComponent::OnTouchEnd(Touch* touch)
{
    
}
