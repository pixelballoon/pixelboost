#include <string>

#include "pixelboost/maths/boundingBox.h"

#include "command/standard/modifyDepth.h"
#include "core/uidHelpers.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"
#include "view/entity/entity.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

Command* ModifyDepthCommand::Create()
{
    return new ModifyDepthCommand();
}

std::string ModifyDepthCommand::GetStaticName()
{
    return "modifyDepth";
}

ModifyDepthCommand::ModifyDepthCommand()
{
    
}

ModifyDepthCommand::~ModifyDepthCommand()
{
}

std::string ModifyDepthCommand::GetName()
{
    return ModifyDepthCommand::GetStaticName();
}

bool ModifyDepthCommand::CanUndo()
{
    return false;
}

bool ModifyDepthCommand::Do(std::string& returnString)
{
    Core* core = Core::Instance();
    Level* level = View::Instance()->GetLevel();
    
    Selection& selection = core->GetSelection();
    
    pb::BoundingBox selectionBounds;
    
    const Selection::Entities& entities = selection.GetSelection();
    
    float selectionMin = +INFINITY;
    float selectionMax = -INFINITY;
    
    for (Selection::Entities::const_iterator it = entities.begin(); it != entities.end(); ++it)
    {
        ViewEntity* entity = level->GetEntityById(it->first);
        selectionBounds.Expand(entity->GetBoundingBox());
        
        selectionMin = glm::min(selectionMin, entity->GetPosition().z);
        selectionMax = glm::max(selectionMax, entity->GetPosition().z);
    }
    
    float offset = 0.f;
    bool modify = false;
    float otherMin = +INFINITY;
    float otherMax = -INFINITY;
    
    Level::EntityList boundsEntities = level->GetEntitiesInBounds(selectionBounds);
    
    for (Level::EntityList::iterator it = boundsEntities.begin(); it != boundsEntities.end(); ++it)
    {
        ViewEntity* entity = *it;
        if (!selection.IsSelected(GenerateSelectionUid(entity->GetUid())))
        {
            modify = true;
            otherMin = glm::min(otherMin, entity->GetPosition().z);
            otherMax = glm::max(otherMax, entity->GetPosition().z);
        }
    }
    
    if (IsArgumentSet("f"))
    {
        offset = (otherMax + 1.f) - selectionMin;
    } else if (IsArgumentSet("b"))
    {
        offset = (otherMin - 1.f) - selectionMax;
    }
    
    if (modify && offset != 0.f)
    {
        for (Selection::Entities::const_iterator it = entities.begin(); it != entities.end(); ++it)
        {
            ViewEntity* entity = level->GetEntityById(it->first);
            entity->Transform(glm::vec3(0,0,offset));
            entity->CommitTransform();
        }
    }

    return true;
}
