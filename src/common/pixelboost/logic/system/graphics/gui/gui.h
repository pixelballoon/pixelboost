#pragma once

#include <set>
#include <stack>
#include <vector>

#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/logic/system.h"

#define InternalPbStringify(value) #value
#define InternalPbToString(value) InternalPbStringify(value)

#define PbGuiId(message, userData) { pb::TypeHash(__FILE__ InternalPbToString(__LINE__)), message.GetGuiComponent(), userData }

namespace pb
{
    class GuiComponent;
    class GuiRenderMessage;
    
    struct GuiId
    {
        operator bool() { return FileHash || Component || UserData; }
        bool operator==(const GuiId& other) const { return FileHash == other.FileHash && Component == other.Component && UserData == other.UserData; }
        bool operator<(const GuiId& other) const { return FileHash < other.FileHash || Component < other.Component || UserData < other.UserData; }
        
        pb::Uid FileHash;
        pb::GuiComponent* Component;
        void* UserData;
    };
    
    struct GuiInputEvent
    {
        enum InputEventType
        {
            kInputEventMouse,
            kInputEventKeyboard,
        } Type;
        
        ModifierKeys Modifier;
        
        union
        {
            KeyboardEvent Keyboard;
            MouseEvent Mouse;
        };
    };
    
    struct GuiLayoutHint
    {
        enum HintType
        {
            kTypeMinWidth,
            kTypeMinHeight,
            kTypeMaxWidth,
            kTypeMaxHeight,
            kTypeFillWidth,
            kTypeFillHeight,
        } Type;
        
        float Value;
        
        GuiLayoutHint(HintType type, float value) { Type = type; Value = value; }
    };
    
    struct GuiLayout
    {
        glm::vec2 Position;
        glm::vec2 Size;
    };
    
    struct GuiLayoutArea
    {
        enum LayoutType
        {
            kLayoutTypeVertical,
            kLayoutTypeHorizontal,
        } Type;
        
        glm::vec2 Position;
        glm::vec2 Size;
        glm::vec2 Pointer;
    };
    
    struct GuiSkin
    {
        glm::vec2 Padding;
    };
    
    struct GuiState
    {
        struct GuiItem
        {
            GuiId Item;
            bool Active;
            int Component;
        };
        
        GuiItem Hot;
        GuiItem Active;
        GuiItem Keyboard;
        
        GuiSkin Skin;
        
        glm::vec2 MousePosition;
        bool MouseDown;
        bool MousePressed;
        bool MouseReleased;
        
        std::stack<GuiLayoutArea> LayoutStack;
    };
    
    class GuiRenderSystem : public SceneSystem, public MouseHandler, public KeyboardHandler
    {
    public:
        GuiRenderSystem();
        virtual ~GuiRenderSystem();
        
        virtual Uid GetType() const;
        static Uid GetStaticType();
        
        virtual void Update(Scene* scene, float totalTime, float gameTime);
        virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);
        
        void PushLayoutArea(GuiLayoutArea layout);
        GuiLayoutArea PopLayoutArea();
        
        void AddLayout(GuiId guiId, const std::vector<GuiLayoutHint> hints, glm::vec2 size);
        GuiLayout GetLayout(GuiId guiId);
        
    private:
        void AddGui(GuiComponent* component);
        void RemoveGui(GuiComponent* component);
        
        virtual int GetInputHandlerPriority();
        
        virtual bool OnMouseEvent(MouseEvent event);
        virtual bool OnKeyboardEvent(KeyboardEvent event);
        
        std::vector<GuiInputEvent> _InputEvents;
        std::set<GuiComponent*> _GuiItems;
        
        std::map<GuiId, GuiLayout> _GuiLayout;
        GuiState _State;
        
        friend class GuiComponent;
    };
    
}
