#include <vector>

#include "project/definitions.h"
#include "view/manipulator/manipulator.h"

class ScaleManipulator : public Manipulator
{
    PB_DECLARE_ENTITY
    
protected:
    ScaleManipulator(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~ScaleManipulator();
    
public:  
    virtual std::string GetName();
    virtual char GetKey();
    
    virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
    virtual bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
    virtual bool OnMouseMove(glm::vec2 position);
    virtual bool OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character);
    virtual bool OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character);
    
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
