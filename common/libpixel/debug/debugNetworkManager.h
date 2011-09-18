#ifndef LIBPIXEL__DEBUG__DEBUGNETWORKMANAGER__H
#define LIBPIXEL__DEBUG__DEBUGNETWORKMANAGER__H

#include <map>
#include <queue>
#include <vector>

namespace libpixel
{
    class DebugConnection;
    class DebugNetworkManager;
    class NetworkMessage;
    
    class DebugNetworkHandler
    {
    public:
        DebugNetworkHandler(int protocol);
        virtual ~DebugNetworkHandler();
        
        int GetProtocol();

        virtual void OnConnectionOpened(DebugConnection& connection);
        virtual void OnConnectionClosed(DebugConnection& connection);
        
        virtual void OnReceive(DebugConnection& connection, NetworkMessage& message);
        
    private:
        int _Protocol;
    };
    
    class DebugConnection
    {
    public:
        DebugConnection(DebugNetworkManager* networkManager);
        ~DebugConnection();
        
        void Close();
        
        bool IsOpen();
        
        int GetConnectionId();
        
        void Update();
        
        void Send(NetworkMessage& message);
        
    private:
        void ProcessSend();
        void ProcessRecv();
        
        void SetConnection(int connection);
        
        DebugNetworkManager* _NetworkManager;
        
        bool _IsReading;
        bool _IsOpen;
        
        int _Connection;
        
        std::queue<NetworkMessage*> _SendQueue;
        
        char* _SendBuffer;
        int _SendOffset;
        int _SendLength;
        
        char* _RecvBuffer;
        int _RecvOffset;
        int _RecvLength;
        
        friend class DebugNetworkManager;
    };
    
    class DebugNetworkManager
    {
    public:
        DebugNetworkManager();
        ~DebugNetworkManager();
        
        static DebugNetworkManager* Instance();
        
        void StartServer(int port, int maxConnections=8);
        void StopServer();
        
        void OpenClient(const char* host, int port);
        void CloseClient();
        
        DebugConnection& GetClientConnection();
        
        void Update();
        
        bool RegisterHandler(DebugNetworkHandler* handler);
        
        void SendMessage(NetworkMessage& message);
        
    private:
        void ConnectionOpened(DebugConnection& connection);
        void ConnectionClosed(DebugConnection& connection);
        void HandleMessage(DebugConnection& connection, NetworkMessage& message);
        
    private:
        typedef std::map<int, DebugNetworkHandler*> HandlerMap;
        typedef std::vector<DebugConnection*> ConnectionList;
        
    private:
        enum State
        {
            kStateStarting,
            kStateBinding,
            kStateListening,
            kStateConnecting,
            kStateConnected,
            kStateDisconnecting,
            kStateDisconnected
        };
        
        HandlerMap _Handlers;
        
        State _ServerState;
        State _ClientState;
        
        int _ServerSocket;
        int _ClientSocket;
        
        int _ServerPort;
        
        char* _ClientHost;
        int _ClientPort;
        
        DebugConnection* _ClientConnection;
        ConnectionList _ServerConnections;
        
        int _MaxConnections;
        
        static DebugNetworkManager* _Instance;
        
        friend class DebugConnection;
    };
    
}

#endif