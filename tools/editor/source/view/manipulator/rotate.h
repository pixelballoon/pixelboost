#include <vector>

#include "project/definitions.h"
#include "view/manipulator/manipulator.h"

class RotateManipulator : public Manipulator
{
    PB_DECLARE_ENTITY
    
protected:
    RotateManipulator(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~RotateManipulator();
    
public:
    virtual std::string GetManipulatorName();
    virtual char GetKey();
    
    virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
    virtual bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
    virtual bool OnMouseMove(glm::vec2 position);
    virtual bool OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character);
    virtual bool OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character);
    
    virtual void OnSetActive();
    virtual void OnSetInactive();
    
    virtual void DoGui(const pb::GuiRenderMessage& guiRenderMessage);
    
    glm::vec3 GetSnap() const;
    void SetSnap(glm::vec3 snap);
    
private:
    void Commit();
    void Reset();
    
private:
    typedef std::vector<Uid> EntityList;
    
    bool _Active;
    glm::vec3 _Center;
    glm::vec2 _Start;
    glm::vec2 _End;
    
    glm::vec3 _Snap;
    
    EntityList _EntityIds;
};
