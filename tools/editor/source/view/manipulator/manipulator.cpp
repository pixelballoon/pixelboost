#include "pixelboost/logic/scene.h"

#include "view/manipulator/manipulator.h"
#include "view.h"

class ManipulatorKeyboardHandler : public pb::KeyboardHandler
{
public:
    ManipulatorKeyboardHandler()
    {
    }
    
    ~ManipulatorKeyboardHandler()
    {
    }
    
    virtual int GetInputHandlerPriority()
    {
        return 1;
    }
    
    virtual bool OnKeyboardEvent(pb::KeyboardEvent event)
    {
        switch (event.Type)
        {
            case pb::KeyboardEvent::kKeyboardEventDown:
            {
                View::Instance()->GetManipulatorManager()->OnKeyDown(event.Key, event.Modifier, event.Character);
                break;
            }
            case pb::KeyboardEvent::kKeyboardEventUp:
            {
                View::Instance()->GetManipulatorManager()->OnKeyUp(event.Key, event.Modifier, event.Character);
                break;
            }
        }
        
        return false;
    }
};

class ManipulatorMouseHandler : public pb::MouseHandler
{
public:
    ManipulatorMouseHandler()
    {
    }
    
    ~ManipulatorMouseHandler()
    {
    }
    
    virtual int GetInputHandlerPriority()
    {
        return 1;
    }
    
    virtual bool OnMouseEvent(pb::MouseEvent event)
    {
        switch (event.Type)
        {
            case pb::MouseEvent::kMouseEventDown:
            {
                return View::Instance()->GetManipulatorManager()->OnMouseDown(event.Down.Button, event.Down.Modifier, glm::vec2(event.Down.Position[0], event.Down.Position[1]));
            }
            case pb::MouseEvent::kMouseEventUp:
            {
                return View::Instance()->GetManipulatorManager()->OnMouseUp(event.Up.Button, event.Up.Modifier, glm::vec2(event.Up.Position[0], event.Up.Position[1]));
            }
            case pb::MouseEvent::kMouseEventMove:
            {
                return View::Instance()->GetManipulatorManager()->OnMouseMove(glm::vec2(event.Move.Position[0], event.Move.Position[1]));
            }
            default:
                return false;
        }
    }
};

PB_DEFINE_ENTITY_ABSTRACT(Manipulator)
    
Manipulator::Manipulator(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : pb::Entity(scene, parent, creationEntity)
{
    
}

Manipulator::~Manipulator()
{
    
}

char Manipulator::GetKey()
{
    return 0;
}

void Manipulator::Render(int layer)
{
    
}

void Manipulator::OnSetActive()
{
    
}

void Manipulator::OnSetInactive()
{
    
}

void Manipulator::DoGui(const pb::GuiRenderMessage& guiRenderMessage)
{
    
}

ManipulatorManager::ManipulatorManager(pb::Scene* scene)
    : _ActiveManipulator(0)
    , _Scene(scene)
{
    _KeyboardHandler = new ManipulatorKeyboardHandler();
    _MouseHandler = new ManipulatorMouseHandler();
    
    View::Instance()->GetKeyboardManager()->AddHandler(_KeyboardHandler);
    View::Instance()->GetMouseManager()->AddHandler(_MouseHandler);
}

ManipulatorManager::~ManipulatorManager()
{
    View::Instance()->GetKeyboardManager()->RemoveHandler(_KeyboardHandler);
    View::Instance()->GetMouseManager()->RemoveHandler(_MouseHandler);
    
    delete _KeyboardHandler;
    delete _MouseHandler;
    
    for (ManipulatorMap::iterator it = _Manipulators.begin(); it != _Manipulators.end(); ++it)
    {
        _Scene->DestroyEntity(it->second);
    }
}

void ManipulatorManager::Render(int layer)
{
    _ActiveManipulator->Render(layer);
}

void ManipulatorManager::AddManipulator(Manipulator* manipulator)
{
    _Manipulators[manipulator->GetManipulatorName()] = manipulator;
}

Manipulator* ManipulatorManager::GetManipulator(const std::string& name)
{
    return _Manipulators[name];
}

Manipulator* ManipulatorManager::GetActiveManipulator()
{
    return _ActiveManipulator;
}

Manipulator* ManipulatorManager::SetActiveManipulator(const std::string& name)
{
    ManipulatorMap::iterator it = _Manipulators.find(name);
    
    if (it == _Manipulators.end())
        return 0;
    
    if (_ActiveManipulator)
        _ActiveManipulator->OnSetInactive();
    
    _ActiveManipulator = it->second;
    _ActiveManipulator->OnSetActive();
    
    return _ActiveManipulator;
}

bool ManipulatorManager::OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    return _ActiveManipulator->OnMouseDown(button, modifierKeys, position);
}

bool ManipulatorManager::OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    return _ActiveManipulator->OnMouseUp(button, modifierKeys, position);
}

bool ManipulatorManager::OnMouseMove(glm::vec2 position)
{
    return _ActiveManipulator->OnMouseMove(position);
}

bool ManipulatorManager::OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character)
{
    if (_ActiveManipulator->OnKeyDown(key, modifier, character))
        return true;
    
    if (modifier == pb::kModifierKeyNone && key == pb::kKeyboardKeyCharacter)
    {
        for (ManipulatorMap::iterator it = _Manipulators.begin(); it != _Manipulators.end(); ++it)
        {
            if (character == it->second->GetKey())
            {
                SetActiveManipulator(it->second->GetManipulatorName());
                return true;
            }
        }
    }
    
    return false;
}

bool ManipulatorManager::OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character)
{
    return _ActiveManipulator->OnKeyUp(key, modifier, character);
}
