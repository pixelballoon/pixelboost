#include <string>
#include <vector>

#include "view/manipulator/manipulator.h"

class CreateManipulator : public Manipulator
{
    PB_DECLARE_ENTITY
    
public:
    CreateManipulator(pb::Scene* scene, pb::Entity* entity, pb::DbEntity* creationEntity);
    virtual ~CreateManipulator();
    
    virtual std::string GetManipulatorName();
    
public:
    void SetEntityType(const std::string& entity);
    void SetCreationData(const std::vector<std::string>& fields, const std::vector<std::string>& values);
    
public:
    virtual bool OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
    virtual bool OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position);
    virtual bool OnMouseMove(glm::vec2 position);
    virtual bool OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character);
    virtual bool OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character);
    
private:
    glm::vec3 GetEntityPosition(glm::vec2 start);
    void CreateEntity(glm::vec3 position);
    
private:
    bool _CreateMode;
    glm::vec3 _LastCreate;
    
    std::string _EntityType;
    std::vector<std::string> _Fields;
    std::vector<std::string> _Values;
};
