#include <stdlib.h>

#include "pixelboost/db/database.h"
#include "pixelboost/debug/debugDatabaseHandler.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/network/networkMessage.h"

using namespace pb;

DebugDatabaseHandler::DebugDatabaseHandler()
    : NetworkHandler('DBDG')
{
    pb::FileSystem::Instance()->CreateDirectory("/gamedata/records/");
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
    const char* messageFile;
    message.ReadString(messageFile);
    std::string filename = messageFile;
    
    const char* contents;
    message.ReadString(contents);
    
    pb::File* file = pb::FileSystem::Instance()->OpenFile("data/gamedata/" + filename, pb::kFileModeWrite);
    if (file)
    {
        file->Write(contents);
        delete file;
    }
    
    if (filename == "main.lua")
    {
        Database::Instance()->OpenDatabase();
    } else if (filename.substr(0,8) == "records/")
    {
        std::string recordIdStr = filename.substr(8, 8);
        Uid recordId = strtoul(recordIdStr.c_str(), 0, 16);
        Database::Instance()->OpenRecord(recordId);
    }
}
