#pragma once

#include "pixelboost/network/networkServer.h"

namespace pb
{
    class DebugDatabaseHandler : public NetworkHandler
    {
    public:
        DebugDatabaseHandler();
        virtual ~DebugDatabaseHandler();
        
        virtual void OnConnectionOpened(NetworkConnection& connection);
        virtual void OnConnectionClosed(NetworkConnection& connection);
        
        virtual void OnReceive(NetworkConnection& connection, NetworkMessage& message);
    };
}