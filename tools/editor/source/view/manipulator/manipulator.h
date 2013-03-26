#pragma once

#include <map>
#include <string>

#include "pixelboost/framework/definitions.h"
#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/logic/entity.h"

namespace pb
{
    class GuiRenderMessage;
    class RenderLayer;
}

class ManipulatorKeyboardHandler;
class ManipulatorMouseHandler;

class Manipulator : public pb::Entity
{
    PB_DECLARE_ENTITY
    
protected:
    Manipulator(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~Manipulator();
    
public:
    virtual std::string GetManipulatorName() = 0;
    virtual char GetKey();
    
    virtual void Render(int layer);
    
    virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position) = 0;
    virtual bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position) = 0;
    virtual bool OnMouseMove(glm::vec2 position) = 0;
    virtual bool OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character) = 0;
    virtual bool OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character) = 0;
    
    virtual void OnSetActive();
    virtual void OnSetInactive();
    
    virtual void DoGui(const pb::GuiRenderMessage& guiRenderMessage);
};

class ManipulatorManager
{
public:
    ManipulatorManager(pb::Scene* scene);
    ~ManipulatorManager();
    
public:
    void Render(int layer);
    
public:        
    void AddManipulator(Manipulator* manipulator);
    Manipulator* GetManipulator(const std::string& name);
    Manipulator* GetActiveManipulator();
    
    Manipulator* SetActiveManipulator(const std::string& name);
    
private:
    bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
    bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
    bool OnMouseMove(glm::vec2 position);
    bool OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character);
    bool OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character);
    
private:
    typedef std::map<std::string, Manipulator*> ManipulatorMap;
    
    ManipulatorMap _Manipulators;
    
    ManipulatorKeyboardHandler* _KeyboardHandler;
    ManipulatorMouseHandler* _MouseHandler;
    
    Manipulator* _ActiveManipulator;
    
    pb::Scene* _Scene;
    
    friend class ManipulatorKeyboardHandler;
    friend class ManipulatorMouseHandler;
};
