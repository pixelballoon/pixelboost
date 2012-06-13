#include "command/manager.h"
#include "command/standard/createEntity.h"
#include "command/standard/deleteCommand.h"
#include "command/standard/selectCommand.h"
#include "command/standard/transformCommand.h"
#include "command/standard/file.h"
#include "command/standard/copy.h"
#include "command/standard/paste.h"
#include "core.h"

using namespace pixeleditor;

void Core::RegisterCoreCommands()
{
    REGISTER_COMMAND(OpenCommand);
    REGISTER_COMMAND(CloseCommand);
    REGISTER_COMMAND(SaveCommand);
    REGISTER_COMMAND(ExportCommand);
    
    REGISTER_COMMAND(CreateEntityCommand);
    REGISTER_COMMAND(DeleteCommand);
    REGISTER_COMMAND(SelectCommand);
    
    REGISTER_COMMAND(CopyCommand);
    REGISTER_COMMAND(PasteCommand);
    
    REGISTER_COMMAND(TransformCommand);
}
