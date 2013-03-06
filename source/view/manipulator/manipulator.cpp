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
    
    virtual bool OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character)
    {
        return View::Instance()->GetManipulatorManager()->OnKeyDown(key, modifier, character);
    }
    
    virtual bool OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character)
    {
        return View::Instance()->GetManipulatorManager()->OnKeyUp(key, modifier, character);
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
    
    virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
    {
        return View::Instance()->GetManipulatorManager()->OnMouseDown(button, modifierKeys, position);
    }
    
    virtual bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
    {
        return View::Instance()->GetManipulatorManager()->OnMouseUp(button, modifierKeys, position);
    }
    
    virtual bool OnMouseMove(glm::vec2 position)
    {
        return View::Instance()->GetManipulatorManager()->OnMouseMove(position);
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
