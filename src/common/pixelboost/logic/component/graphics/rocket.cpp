#include "Rocket/Core.h"

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/framework/engine.h"
#include "pixelboost/graphics/renderer/rocket/rocketRenderer.h"
#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/logic/component/graphics/rocket.h"
#include "pixelboost/logic/message/update.h"
#include "pixelboost/logic/system/graphics/render/render.h"
#include "pixelboost/logic/scene.h"

using namespace pb;

namespace pb
{

class RocketKeyboardHandler : public KeyboardHandler
{
public:
    RocketKeyboardHandler(Rocket::Core::Context* context)
        : _Context(context)
    {
        SetPriority(1000);
    }
    
    bool OnKeyDown(KeyboardKey key, char character)
    {
        switch (key)
        {
            case pb::kKeyboardKeyBackspace:
                return _Context->ProcessKeyDown(Rocket::Core::Input::KI_BACK, 0);
                break;
            case pb::kKeyboardKeyCharacter:
                return _Context->ProcessTextInput(character);
                break;
        }
        
        return false;
    }
    
    bool OnKeyUp(KeyboardKey key, char character)
    {
        switch (key)
        {
            case pb::kKeyboardKeyBackspace:
                return _Context->ProcessKeyUp(Rocket::Core::Input::KI_BACK, 0);
                break;
            case pb::kKeyboardKeyCharacter:
                break;
        }
        
        return false;
    }
    
private:
    Rocket::Core::Context* _Context;
};

class RocketMouseHandler : public MouseHandler
{
public:
    RocketMouseHandler(Rocket::Core::Context* context)
        : _Context(context)
    {
        SetPriority(1000);
        
        _WheelDelta = 0.f;
    }
    
    bool OnMouseDown(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
    {
        OnMouseMove(position);
        _Context->ProcessMouseButtonDown(button, 0);
        
        Rocket::Core::Element* focus = _Context->GetFocusElement();
        Rocket::Core::Element* hover = _Context->GetHoverElement();
        
        if (_Context->GetHoverElement() != _Context->GetRootElement())
            return true;
        
        return false;
    }
    
    bool OnMouseMove(glm::vec2 position)
    {
        _Context->ProcessMouseMove(position.x, position.y, 0);
        return false;
    }
    
    bool OnMouseUp(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
    {
        OnMouseMove(position);
        _Context->ProcessMouseButtonUp(button, 0);
        return false;
    }
    
    bool OnMouseScroll(ModifierKeys modifierKeys, glm::vec2 scroll)
    {
        float scrollValue = -scroll.y/10.f;
        _WheelDelta += scrollValue;
        
        float wheelRemainder = glm::mod(glm::abs(_WheelDelta), 1.f) * (_WheelDelta > 0.f ? 1.f : -1.f);
        
        scrollValue = _WheelDelta - wheelRemainder;
        
        _WheelDelta = wheelRemainder;
        
        return _Context->ProcessMouseWheel(scrollValue, 0);
    }
    
    bool OnMouseZoom(glm::vec2 zoom)
    {
        return false;
    }
    
    bool OnMouseRotate(float rotate)
    {
        return false;
    }
    
private:
    float _WheelDelta;
    glm::vec2 _PrevMouse;
    
    Rocket::Core::Context* _Context;
};
    
}

RocketComponent::RocketComponent(Entity* entity, glm::vec2 size)
    : Component(entity)
{
    _Context = Rocket::Core::CreateContext("default", Rocket::Core::Vector2i(size.x, size.y));
    
    _Renderable = new RocketRenderable(_Context);
    _Renderable->SetRenderPass(pb::kRenderPassUi);
    GetScene()->GetSystemByType<pb::RenderSystem>()->AddItem(_Renderable);

    _KeyboardHandler = new RocketKeyboardHandler(_Context);
    _MouseHandler = new RocketMouseHandler(_Context);
    
    Engine::Instance()->GetKeyboardManager()->AddHandler(_KeyboardHandler);
    Engine::Instance()->GetMouseManager()->AddHandler(_MouseHandler);
    
    GetParent()->RegisterMessageHandler<UpdateMessage>(Entity::MessageHandler(this, &RocketComponent::OnUpdate));
}

RocketComponent::~RocketComponent()
{
    Engine::Instance()->GetKeyboardManager()->RemoveHandler(_KeyboardHandler);
    Engine::Instance()->GetMouseManager()->RemoveHandler(_MouseHandler);
    
    GetParent()->UnregisterMessageHandler<UpdateMessage>(Entity::MessageHandler(this, &RocketComponent::OnUpdate));
}

Uid RocketComponent::GetType()
{
    return RocketComponent::GetStaticType();
}

Uid RocketComponent::GetStaticType()
{
    return TypeHash("pb::RocketComponent");
}

void RocketComponent::SetLayer(int layer)
{
    _Renderable->SetLayer(layer);
}

int RocketComponent::GetLayer()
{
    return _Renderable->GetLayer();
}

Rocket::Core::Context* RocketComponent::GetContext()
{
    return _Context;
}

void RocketComponent::LoadRML(FileLocation location, const std::string& filename)
{
    std::string file = pb::FileHelpers::GetRootPath()+filename;
    Rocket::Core::ElementDocument* document = _Context->LoadDocument(file.c_str());
    if (document != NULL)
    {
        document->Show();
        document->RemoveReference();
    }
    
}

void RocketComponent::OnUpdate(const pb::Message& message)
{
    _Context->Update();
}
