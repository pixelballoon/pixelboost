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

class pb::RocketKeyboardHandler : public KeyboardHandler
{
public:
    RocketKeyboardHandler(Rocket::Core::Context* context)
        : _Context(context)
    {
        SetPriority(1001);
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

class pb::RocketMouseHandler : public MouseHandler
{
public:
    RocketMouseHandler(Rocket::Core::Context* context)
        : _Context(context)
    {
        SetPriority(1001);
    }
    
    bool OnMouseDown(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position)
    {
        OnMouseMove(position);
        _Context->ProcessMouseButtonDown(button, 0);
        
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
        return _Context->ProcessMouseWheel(scroll.y*120, 0);
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
    glm::vec2 _PrevMouse;
    
    Rocket::Core::Context* _Context;
};

RocketComponent::RocketComponent(Entity* entity, glm::vec2 size)
    : Component(entity)
{
    _Context = Rocket::Core::CreateContext("default", Rocket::Core::Vector2i(size.x, size.y));
    
    _Renderable = new RocketRenderable(_Context);
    _Renderable->SetLayer(3);
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

Rocket::Core::Context* RocketComponent::GetContext()
{
    return _Context;
}

void RocketComponent::OnUpdate(const pb::Message& message)
{
    _Context->Update();
}
