#pragma once

#include <set>
#include <string>

#include "pixelboost/input/keyboardManager.h"
#include "pixelboost/input/mouseManager.h"
#include "pixelboost/logic/system.h"

#define InternalPbStringify(value) #value
#define InternalPbToString(value) InternalPbStringify(value)

#define PbGuiId(message, userData) pb::GuiId(pb::TypeHash(__FILE__ InternalPbToString(__LINE__)), message.GetGuiComponent(), (void*)(userData))
#define PbNestedGuiId(id, userData) pb::GuiId(id, pb::TypeHash(__FILE__ InternalPbToString(__LINE__))+userData)

namespace pb
{
    class GuiComponent;
    class GuiRenderMessage;
    
    struct GuiLayoutArea;
    struct GuiLayout;
    
    struct GuiId
    {
    public:
        GuiId()
        {
            FileHash = 0;
            Component = 0;
            UserData = 0;
            NestedHash = 0;
        }
        GuiId(const GuiId& guiId, pb::Uid fileHash)
        {
            *this = guiId;
            NestedHash += fileHash;
        }
        GuiId(pb::Uid fileHash, pb::GuiComponent* component, void* userData)
        {
            FileHash = fileHash;
            Component = component;
            UserData = userData;
            NestedHash = 0;
        }
        ~GuiId() {}
        
        operator bool() { return FileHash || Component || UserData; }
        bool operator==(const GuiId& other) const { return FileHash == other.FileHash && Component == other.Component && UserData == other.UserData && NestedHash == other.NestedHash; }
        bool operator<(const GuiId& other) const
        {
            if (FileHash != other.FileHash) {
                return FileHash < other.FileHash;
            } else if (Component != other.Component) {
                return Component < other.Component;
            } else if (NestedHash != other.NestedHash) {
                return NestedHash < other.NestedHash;
            } else
            {
                return UserData < other.UserData;
            }
        }
        
        pb::Uid FileHash;
        pb::GuiComponent* Component;
        void* UserData;
        pb::Uid NestedHash;
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
        GuiLayout(const std::string& tag);
        void ProcessHints(const std::vector<GuiLayoutHint>& hints, glm::vec2 size);
        
        enum LayoutType
        {
            kLayoutTypeVertical,
            kLayoutTypeHorizontal,
        } LayoutType;
        
        bool Valid;
        
        std::string Tag;
        
        GuiLayout* Parent;
        std::vector<GuiLayout*> Children;
        
        glm::vec2 Scroll;
        glm::vec2 Position;
        
        glm::vec2 LocalPosition;
        glm::vec2 Size;
        glm::vec2 ContentsSize;
        
        glm::vec2 MinSize;
        glm::vec2 MaxSize;
        
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
        glm::vec2 MouseWheel;
        bool MouseDown;
        bool MousePressed;
        bool MouseReleased;
        
        KeyboardEvent KeyboardEvent;

        std::vector<GuiLayout*> LayoutStack;
    };
    
    struct GuiData
    {
        union GuiDataItem
        {
            bool Bool;
            float Float;
        };
        
        bool Active;
        bool Initialised;
        GuiDataItem Data[8];
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
        
        void AddLayout(const std::string& tag, GuiId guiId, const std::vector<GuiLayoutHint> hints, glm::vec2 size);
        
        GuiLayout* GetLayout(GuiId guiId);
        GuiData& GetData(GuiId guiId);
        
    private:
        void ProcessLayouts();
        bool ProcessLayout(GuiLayout* layout, glm::vec2 position, bool positionValid);
        
        void Clear();
        void ClearLayout(GuiLayout* layout);
        
        void AddGui(GuiComponent* component);
        void RemoveGui(GuiComponent* component);
        
        virtual int GetInputHandlerPriority();
        
        virtual bool OnMouseEvent(MouseEvent event);
        virtual bool OnKeyboardEvent(KeyboardEvent event);
        
        std::vector<GuiInputEvent> _InputEvents;
        std::set<GuiComponent*> _GuiItems;
        
        std::map<GuiId, GuiData> _GuiData;
        
        std::map<GuiId, GuiLayout*> _GuiArea;
        std::map<GuiId, GuiLayout*> _GuiLayout;
        GuiState _State;
        
        friend class GuiComponent;
    };
    
}
