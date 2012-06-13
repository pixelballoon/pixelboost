#include <vector>

#include "project/definitions.h"
#include "view/manipulator/manipulator.h"

namespace pixeleditor
{
    class ScaleManipulator : public Manipulator
    {
    public:
        ScaleManipulator(pb::Scene* scene);
        virtual ~ScaleManipulator();
        
        virtual std::string GetName();
        virtual char GetKey();
        
        virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
        virtual bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
        virtual bool OnMouseMove(glm::vec2 position);
        virtual bool OnKeyDown(pb::KeyboardKey key, char character);
        virtual bool OnKeyUp(pb::KeyboardKey key, char character);
        
        virtual void OnSetActive();
        virtual void OnSetInactive();
        
    private:
        void Commit();
        void Reset();
        
    private:
        typedef std::vector<Uid> EntityList;
        
        bool _Active;
        glm::vec3 _Center;
        glm::vec2 _Start;
        glm::vec2 _End;
        
        EntityList _EntityIds;
    };
}
