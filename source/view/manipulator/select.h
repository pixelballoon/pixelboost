#include "view/manipulator/manipulator.h"

class SelectManipulator : public Manipulator
{
public:
    SelectManipulator(pb::Scene* scene);
    virtual ~SelectManipulator();
    
    virtual pb::Uid GetType() const;
    static pb::Uid GetStaticType();
    
    virtual std::string GetName();
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
    bool _Active;
    glm::vec2 _SelectStart;
    glm::vec2 _SelectEnd;
    
    pb::RectangleComponent* _BoundsComponent;
};
