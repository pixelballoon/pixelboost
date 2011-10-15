#ifndef LIBPIXEL__NETWORK__NETWORKSERVER__H
#define LIBPIXEL__NETWORK__NETWORKSERVER__H

#include <map>
#include <queue>
#include <vector>

namespace libpixel
{
    class NetworkConnection;
    class NetworkMessage;
    class NetworkServer;
    
    class NetworkHandler
    {
    public:
        NetworkHandler(int protocol);
        virtual ~NetworkHandler();
        
        int GetProtocol();
        
        virtual void OnConnectionOpened(NetworkConnection& connection);
        virtual void OnConnectionClosed(NetworkConnection& connection);
        
        virtual void OnReceive(NetworkConnection& connection, NetworkMessage& message);
        
    private:
        int _Protocol;
    };
    
    class NetworkConnection
    {
    public:
        NetworkConnection(NetworkServer* networkManager);
        ~NetworkConnection();
        
        void Close();
        
        bool IsOpen();
        
        int GetConnectionId();
        
        void Update();
        
        void Send(NetworkMessage& message);
        
    private:
        void ProcessSend();
        void ProcessRecv();
        
        void SetConnection(int connection);
        
        NetworkServer* _NetworkManager;
        
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
        
        friend class NetworkServer;
    };
    
    class NetworkServer
    {
    public:
        NetworkServer();
        ~NetworkServer();
        
        static NetworkServer* Instance();
        
        void StartServer(int port, int maxConnections=8);
        void StopServer();
        
        void OpenClient(const char* host, int port);
        void CloseClient();
        
        NetworkConnection& GetClientConnection();
        
        void Update();
        
        bool RegisterHandler(NetworkHandler* handler);
        
        void SendMessage(NetworkMessage& message);
        
    private:
        void ConnectionOpened(NetworkConnection& connection);
        void ConnectionClosed(NetworkConnection& connection);
        void HandleMessage(NetworkConnection& connection, NetworkMessage& message);
        
    private:
        typedef std::map<int, NetworkHandler*> HandlerMap;
        typedef std::vector<NetworkConnection*> ConnectionList;
        
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
        
        NetworkConnection* _ClientConnection;
        ConnectionList _ServerConnections;
        
        int _MaxConnections;
        
        static NetworkServer* _Instance;
        
        friend class NetworkConnection;
    };
    
}

#endif