#pragma once

#include <set>
#include <string>

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
    
    struct GuiLayoutArea;
    struct GuiLayout;
    
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
            kTypeFixedWidth,
            kTypeFixedHeight,
            kTypeMinWidth,
            kTypeMinHeight,
            kTypeMaxWidth,
            kTypeMaxHeight,
            kTypeExpandWidth,
            kTypeExpandHeight,
        } Type;
        
        union
        {
            float Float;
            bool Bool;
        };
        
        GuiLayoutHint(HintType type, float value=0.f) { Type = type; Float = value; }
        GuiLayoutHint(HintType type, bool value) { Type = type; Bool = value; }
        
        static GuiLayoutHint Width(float width) { return GuiLayoutHint(kTypeFixedWidth, width); }
        static GuiLayoutHint Height(float height) { return GuiLayoutHint(kTypeFixedHeight, height); }
        static GuiLayoutHint ExpandWidth(bool expand = true) { return GuiLayoutHint(kTypeExpandWidth, expand); }
        static GuiLayoutHint ExpandHeight(bool expand = true) { return GuiLayoutHint(kTypeExpandHeight, expand); }
    };
    
    struct GuiLayout
    {
        GuiLayout();
        void ProcessHints(const std::vector<GuiLayoutHint>& hints, glm::vec2 size);
        
        enum LayoutType
        {
            kLayoutTypeVertical,
            kLayoutTypeHorizontal,
        } LayoutType;
        
        GuiLayout* Parent;
        std::vector<GuiLayout*> Children;
        
        glm::vec2 Position;
        glm::vec2 Size;
        
        int ChildrenPending;
        glm::vec2 RemainingSize;
        
        bool PendingPosition;
        bool PendingSize;
        
        bool HasMinMax;
        glm::vec4 MinMaxSize;
        bool Expand[2];
    };
        
    struct GuiSkin
    {
        std::string Font;
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

        std::vector<GuiLayout*> LayoutStack;
    };
    
    class GuiSystem : public SceneSystem, public MouseHandler, public KeyboardHandler
    {
    public:
        GuiSystem();
        virtual ~GuiSystem();
        
        virtual Uid GetType() const;
        static Uid GetStaticType();
        
        virtual void Update(Scene* scene, float totalTime, float gameTime);
        virtual void Render(Scene* scene, Viewport* viewport, RenderPass renderPass);
        
        void PushLayoutArea(const GuiLayout& layout, GuiId guiId, const std::vector<GuiLayoutHint> hints);
        GuiLayout* PopLayoutArea();
        GuiLayout* GetLayoutArea(GuiId guiId);
        
        void AddLayout(GuiId guiId, const std::vector<GuiLayoutHint> hints, glm::vec2 size);
        GuiLayout* GetLayout(GuiId guiId);
        
    private:
        struct LayoutResult
        {
            bool Pending;
            glm::vec2 Size;
        };
        
        void ProcessLayouts();
        LayoutResult ProcessLayout(GuiLayout* layout, glm::vec2 position);
        
        void Clear();
        void ClearLayout(GuiLayout* layout);
        
        void AddGui(GuiComponent* component);
        void RemoveGui(GuiComponent* component);
        
        virtual int GetInputHandlerPriority();
        
        virtual bool OnMouseEvent(MouseEvent event);
        virtual bool OnKeyboardEvent(KeyboardEvent event);
        
        std::vector<GuiInputEvent> _InputEvents;
        std::set<GuiComponent*> _GuiItems;
        
        std::map<GuiId, GuiLayout*> _GuiArea;
        std::map<GuiId, GuiLayout*> _GuiLayout;
        GuiState _State;
        
        friend class GuiComponent;
    };
    
}
