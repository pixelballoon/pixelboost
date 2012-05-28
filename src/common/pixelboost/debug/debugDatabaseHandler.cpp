#include "pixelboost/db/database.h"
#include "pixelboost/debug/debugDatabaseHandler.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/network/networkMessage.h"

using namespace pb;

DebugDatabaseHandler::DebugDatabaseHandler()
    : NetworkHandler('DBDG')
{
    pb::FileHelpers::CreateDirectory(pb::FileHelpers::GetUserPath() + "/data/gamedata/records/");
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
    
    pb::FileHelpers::StringToFile(pb::FileHelpers::GetUserPath() + "/data/gamedata/" + file, contents);
    
    if (file == "main.lua")
    {
        pb::Database::Instance()->OpenDatabase();
    } else if (file.substr(0,8) == "records/")
    {
        std::string recordIdStr = file.substr(8, 8);
        pb::Uid recordId = strtoull(recordIdStr.c_str(), 0, 16);
        pb::Database::Instance()->OpenRecord(recordId);
        // blah
    }
}
