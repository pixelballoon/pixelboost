#include <string>

#include "command/standard/transformCommand.h"
#include "project/entity.h"
#include "project/project.h"
#include "core.h"

using namespace pixeleditor;
    
Command* TransformCommand::Create()
{
    return new TransformCommand();
}

std::string TransformCommand::GetStaticName()
{
    return "transform";
}

TransformCommand::TransformCommand()
{
    
}

TransformCommand::~TransformCommand()
{
}

std::string TransformCommand::GetName()
{
    return TransformCommand::GetStaticName();
}

bool TransformCommand::CanUndo()
{
    return true;
}

bool TransformCommand::Do(std::string& returnString)
{
    _TransformPosition = false;
    _TransformRotation = false;
    _TransformScale = false;
    
    if (IsArgumentSet("p"))
    {
        _TransformPosition = true;

        _PositionOffset[0] = (float)atof(GetArgument("p", 0).c_str());
        _PositionOffset[1] = (float)atof(GetArgument("p", 1).c_str());
        
        Selection selection = Core::Instance()->GetSelection();
        
        for (Selection::Entities::const_iterator it = selection.GetSelection().begin(); it != selection.GetSelection().end(); ++it)
        {
            Entity* entity = Core::Instance()->GetProject()->GetEntity(it->first);
            
            if (entity)
            {
                glm::vec3 position = entity->GetPosition();
                entity->SetPosition(position+_PositionOffset);
            }
        }
    }
    
    if (IsArgumentSet("r"))
    {
        _TransformRotation = true;
        
        _RotationOffset = (float)atof(GetArgument("r", 0).c_str());
        
        Selection selection = Core::Instance()->GetSelection();
        
        for (Selection::Entities::const_iterator it = selection.GetSelection().begin(); it != selection.GetSelection().end(); ++it)
        {
            Entity* entity = Core::Instance()->GetProject()->GetEntity(it->first);
            
            if (entity)
            {
                float rotation = entity->GetRotation();
                entity->SetRotation(rotation+_RotationOffset);
            }
        }
    }
    
    if (IsArgumentSet("s"))
    {
        _TransformScale = true;
        
        _ScaleOffset[0] = (float)atof(GetArgument("s", 0).c_str());
        _ScaleOffset[1] = (float)atof(GetArgument("s", 1).c_str());
        
        Selection selection = Core::Instance()->GetSelection();
        
        for (Selection::Entities::const_iterator it = selection.GetSelection().begin(); it != selection.GetSelection().end(); ++it)
        {
            Entity* entity = Core::Instance()->GetProject()->GetEntity(it->first);
            
            if (entity)
            {
                glm::vec3 scale = entity->GetScale();
                entity->SetScale(scale*_ScaleOffset);
            }
        }
    }

    return true;
}

bool TransformCommand::Undo()
{
    if (_TransformPosition)
    {
        Selection selection = Core::Instance()->GetSelection();
        
        for (Selection::Entities::const_iterator it = selection.GetSelection().begin(); it != selection.GetSelection().end(); ++it)
        {
            Entity* entity = Core::Instance()->GetProject()->GetEntity(it->first);
            
            if (entity)
            {
                glm::vec3 position = entity->GetPosition();
                entity->SetPosition(position-_PositionOffset);
            }
        }
    }
    
    if (_TransformRotation)
    {
        Selection selection = Core::Instance()->GetSelection();
        
        for (Selection::Entities::const_iterator it = selection.GetSelection().begin(); it != selection.GetSelection().end(); ++it)
        {
            Entity* entity = Core::Instance()->GetProject()->GetEntity(it->first);
            
            if (entity)
            {
                float rotation = entity->GetRotation();
                entity->SetRotation(rotation-_RotationOffset);
            }
        }
    }
    
    if (_TransformScale)
    {
        Selection selection = Core::Instance()->GetSelection();
        
        for (Selection::Entities::const_iterator it = selection.GetSelection().begin(); it != selection.GetSelection().end(); ++it)
        {
            Entity* entity = Core::Instance()->GetProject()->GetEntity(it->first);
            
            if (entity)
            {
                glm::vec3 scale = entity->GetScale();
                entity->SetScale(scale/_ScaleOffset);
            }
        }
    }        
    return true;
}
