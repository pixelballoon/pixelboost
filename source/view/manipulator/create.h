#include <string>

#include "view/manipulator/manipulator.h"

namespace pixeleditor
{
    class CreateManipulator : public Manipulator
    {
    public:
        CreateManipulator();
        virtual ~CreateManipulator();
        
        virtual std::string GetName();
        
    public:
        void SetActorType(const std::string& actor);
        
    public:
        virtual bool OnMouseDown(pb::MouseButton button, glm::vec2 position);
        virtual bool OnMouseUp(pb::MouseButton button, glm::vec2 position);
        virtual bool OnMouseMove(glm::vec2 position);
        virtual bool OnKeyDown(pb::KeyboardKey key, char character);
        virtual bool OnKeyUp(pb::KeyboardKey key, char character);
        
    private:
        std::string _ActorType;
    };
}
