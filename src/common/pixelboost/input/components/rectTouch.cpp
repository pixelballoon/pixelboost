#include "pixelboost/framework/game.h"
#include "pixelboost/graphics/camera/viewport.h"
#include "pixelboost/input/components/rectTouch.h"
#include "pixelboost/input/messages/touch.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/entity.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

RectTouchComponent::RectTouchComponent(Entity* parent)
    : Component(parent)
{
    Game::Instance()->GetTouchManager()->AddHandler(this);
}

RectTouchComponent::~RectTouchComponent()
{
    Game::Instance()->GetTouchManager()->RemoveHandler(this);
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
    
    glm::vec3 position = GetPosition();
    glm::vec2 screenPos = touch.GetViewportPosition();
    glm::vec2 size = _Size/2.f;
    
    if (screenPos.x > position.x-size.x && screenPos.x < position.x+size.x &&
        screenPos.y > position.y-size.y && screenPos.y < position.y+size.y)
    {
        TouchMessage message(GetParent(), this);
        GetScene()->SendMessage(GetParentUid(), message);
        
        return true;
    }
    
    return false;
}

bool RectTouchComponent::OnTouchMove(Touch touch)
{
    return false;
}

bool RectTouchComponent::OnTouchUp(Touch touch)
{
    return false;
}
