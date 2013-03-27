#include "view/manipulator/manipulator.h"

class SelectManipulator : public Manipulator
{
    PB_DECLARE_ENTITY
    
protected:
    SelectManipulator(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    virtual ~SelectManipulator();
    
public:
    virtual std::string GetManipulatorName();
    virtual char GetKey();
    
    virtual void Render(int layer);
    
    virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
    virtual bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
    virtual bool OnMouseMove(glm::vec2 position);
    virtual bool OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character);
    virtual bool OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character);
    
    virtual void OnSetActive();
    virtual void OnSetInactive();
    
private:
    enum Mode
    {
        kModeMove,
        kModeRotate,
        kModeSelect,
    };
    
    bool _Active;
    Mode _Mode;
    glm::vec2 _MouseStart;
    glm::vec2 _MouseEnd;
    
    pb::RectangleComponent* _BoundsComponent;
};
