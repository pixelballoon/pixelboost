#include <stdlib.h>

#include "pixelboost/db/database.h"
#include "pixelboost/debug/debugDatabaseHandler.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/network/networkMessage.h"

using namespace pb;

DebugDatabaseHandler::DebugDatabaseHandler()
    : NetworkHandler('DBDG')
{
    FileHelpers::CreateDirectory(FileHelpers::GetUserPath() + "/data/gamedata/records/");
}

DebugDatabaseHandler::~DebugDatabaseHandler()
{
    
}

void DebugDatabaseHandler::OnConnectionOpened(NetworkConnection& connection)
{
    
}

void DebugDatabaseHandler::OnConnectionClosed(NetworkConnection& connection)
{
    
}

void DebugDatabaseHandler::OnReceive(NetworkConnection& connection, NetworkMessage& message)
{
    const char* filename;
    message.ReadString(filename);
    std::string file = filename;
    
    const char* contents;
    message.ReadString(contents);
    
    FileHelpers::StringToFile(pb::kFileLocationUser, "/data/gamedata/" + file, contents);
    
    if (file == "main.lua")
    {
        Database::Instance()->OpenDatabase();
    } else if (file.substr(0,8) == "records/")
    {
        std::string recordIdStr = file.substr(8, 8);
        Uid recordId = strtoul(recordIdStr.c_str(), 0, 16);
        Database::Instance()->OpenRecord(recordId);
    }
}
