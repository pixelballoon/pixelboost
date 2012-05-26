#include "view/manipulator/manipulator.h"

namespace pixeleditor
{
    class SelectManipulator : public Manipulator
    {
    public:
        SelectManipulator();
        virtual ~SelectManipulator();
        
        virtual std::string GetName();
        virtual char GetKey();
        
        virtual void Render(pb::RenderLayer* layer);
        
        virtual bool OnMouseDown(pb::MouseButton button, glm::vec2 position);
        virtual bool OnMouseUp(pb::MouseButton button, glm::vec2 position);
        virtual bool OnMouseMove(glm::vec2 position);
        virtual bool OnKeyDown(pb::KeyboardKey key, char character);
        virtual bool OnKeyUp(pb::KeyboardKey key, char character);
        
        virtual void OnSetActive();
        virtual void OnSetInactive();
        
    private:
        bool _Active;
        glm::vec2 _SelectStart;
        glm::vec2 _SelectEnd;
    };
}
