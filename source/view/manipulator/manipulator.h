#pragma once

#include <map>
#include <string>

#include "pixelboost/db/definitions.h"
#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"

namespace pb
{
    class RenderLayer;
}

namespace pixeleditor
{
    class ManipulatorKeyboardHandler;
    class ManipulatorMouseHandler;
    
    class Manipulator
    {
    public:
        Manipulator();
        virtual ~Manipulator();
        
        virtual std::string GetName() = 0;
        virtual char GetKey();
        
        virtual void Render(int layer);
        
        virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
        virtual bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
        virtual bool OnMouseMove(glm::vec2 position);
        virtual bool OnKeyDown(pb::KeyboardKey key, char character);
        virtual bool OnKeyUp(pb::KeyboardKey key, char character);
        
        virtual void OnSetActive();
        virtual void OnSetInactive();
    };
    
    class ManipulatorManager
    {
    public:
        ManipulatorManager();
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
        bool OnKeyDown(pb::KeyboardKey key, char character);
        bool OnKeyUp(pb::KeyboardKey key, char character);
        
    private:
        typedef std::map<std::string, Manipulator*> ManipulatorMap;
        
        ManipulatorMap _Manipulators;
        
        ManipulatorKeyboardHandler* _KeyboardHandler;
        ManipulatorMouseHandler* _MouseHandler;
        
        Manipulator* _ActiveManipulator;
        
        friend class ManipulatorKeyboardHandler;
        friend class ManipulatorMouseHandler;
    };
};
