#include <string>

#include "pixelboost/maths/boundingBox.h"

#include "command/standard/checkDepth.h"
#include "core/uidHelpers.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"
#include "view/entity/entity.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

Command* CheckDepthCommand::Create()
{
    return new CheckDepthCommand();
}

std::string CheckDepthCommand::GetStaticName()
{
    return "checkDepth";
}

CheckDepthCommand::CheckDepthCommand()
{
    
}

CheckDepthCommand::~CheckDepthCommand()
{
}

std::string CheckDepthCommand::GetName()
{
    return CheckDepthCommand::GetStaticName();
}

bool CheckDepthCommand::CanUndo()
{
    return false;
}

bool CheckDepthCommand::Do(std::string& returnString)
{
    Level* level = View::Instance()->GetLevel();
    
    for (Level::EntityMap::const_iterator it = level->GetEntities().begin(); it != level->GetEntities().end(); ++it)
    {
        Level::EntityList boundsEntities = level->GetEntitiesInBounds(it->second->GetBoundingBox());
        
        float maxDepth = -INFINITY;
        bool hasConflict = false;
        
        for (Level::EntityList::iterator boundsIt = boundsEntities.begin(); boundsIt != boundsEntities.end(); ++boundsIt)
        {
            ViewEntity* entity = *boundsIt;
            
            if (entity != it->second && entity->GetPosition().z == it->second->GetPosition().z)
            {
                hasConflict = true;
            }
            
            maxDepth = glm::max(maxDepth, entity->GetPosition().z);
        }
        
        if (hasConflict)
        {
            it->second->Transform(glm::vec3(0, 0, (maxDepth + 1) - it->second->GetPosition().z));
            it->second->CommitTransform();
        }
    }
    
    return true;
}
