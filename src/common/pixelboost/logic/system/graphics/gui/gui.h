#include <set>

#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/logic/system.h"

namespace pb
{
    class GuiComponent;
    
    class GuiRenderSystem : public SceneSystem, public MouseHandler, public KeyboardHandler
    {
    public:
        GuiRenderSystem();
        virtual ~GuiRenderSystem();
        
        virtual Uid GetType() const;
        static Uid GetStaticType();
        
        virtual void Update(Scene* scene, float totalTime, float gameTime);
        virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);
        
        bool DoButton(GuiComponent* component, glm::vec2 position, glm::vec2 size);
        
    private:
        void AddGui(GuiComponent* component);
        void RemoveGui(GuiComponent* component);
        
        virtual int GetInputHandlerPriority();
        
        virtual bool OnMouseDown(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position);
        virtual bool OnMouseUp(MouseButton button, ModifierKeys modifierKeys, glm::vec2 position);
        virtual bool OnMouseMove(glm::vec2 position);
        virtual bool OnMouseScroll(ModifierKeys modifierKeys, glm::vec2 delta);
        virtual bool OnMouseZoom(glm::vec2 zoom);
        virtual bool OnMouseRotate(float rotate);
        
        virtual bool OnKeyDown(KeyboardKey key, ModifierKeys modifier, char character);
        virtual bool OnKeyUp(KeyboardKey key, ModifierKeys modifier, char character);
        
        struct InputEvent
        {
            struct InputEventMouse
            {
                enum MouseEventType
                {
                    kMouseEventDown,
                    kMouseEventUp,
                    kMouseEventMove,
                    kMouseEventScroll,
                    kMouseEventZoom,
                    kMouseEventRotate,
                } Type;
                
                MouseButton Button;
                glm::vec2 Position;
                glm::vec2 Delta;
                float Rotate;
            };
            
            struct InputEventKeyboard
            {
                enum KeyboardEventType
                {
                    kKeyboardEventDown,
                    kKeyboardEventUp,
                } Type;
                
                KeyboardKey Key;
                char Character;                
            };
            
            enum InputEventType
            {
                kInputEventMouse,
                kInputEventKeyboard,
            } Type;
            
            ModifierKeys Modifier;
            
            union
            {
                InputEventKeyboard Keyboard;
                InputEventMouse Mouse;
            };
        };
        
        std::vector<InputEvent> _Events;
        std::set<GuiComponent*> _GuiItems;
        
        friend class GuiComponent;
    };
    
}
