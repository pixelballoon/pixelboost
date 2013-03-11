#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "enet/enet.h"

namespace pb
{
    class NetworkConnection;
    class NetworkMessage;
    
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
    
    class NetworkHost
    {
    protected:
        NetworkHost();
        virtual ~NetworkHost();

    public:
        void Close();
        
        bool RegisterHandler(NetworkHandler* handler);
        void BroadcastMessage(NetworkMessage& message);
        
    private:
        void Update();
        
        void HandleMessage(NetworkMessage& message);
        
    protected:
        ENetHost* _Host;
        
        std::vector<NetworkConnection*> _Connections;
        std::map<int, std::vector<NetworkHandler*> > _Handlers;
        
        friend class NetworkManager;
    };
    
    class NetworkClient : public NetworkHost
    {
    protected:
        NetworkClient(const std::string& address, int port);
        ~NetworkClient();
        
    public:
        std::vector<NetworkConnection*> GetConnection();
        
        friend class NetworkManager;
    };
    
    class NetworkServer : public NetworkHost
    {
    protected:
        NetworkServer(int port, int maxConnections);
        ~NetworkServer();
        
    public:
        std::vector<NetworkConnection*> GetConnections();
        
        friend class NetworkManager;
    };
    
    class NetworkDiscovery
    {
    protected:
        NetworkDiscovery(int port);
        virtual ~NetworkDiscovery();
        
        virtual void Update();
        
        int Send(ENetSocket* output, ENetAddress *address, NetworkMessage& message);
        
        virtual void OnMessage(ENetAddress sender, NetworkMessage& message) = 0;
        
    protected:
        int _Port;
        ENetSocket _Socket;
        
        friend class NetworkManager;
    };
    
    class NetworkDiscoveryServer : public NetworkDiscovery
    {
    protected:
        NetworkDiscoveryServer(int port);
        ~NetworkDiscoveryServer();
        
    public:
        NetworkDiscoveryServer* AddService(const std::string& service);
        
    protected:
        virtual void Update();
        virtual void OnMessage(ENetAddress sender, NetworkMessage& message);
        
    protected:
        std::set<std::string> _Services;
        
        friend class NetworkManager;
    };
    
    class NetworkDiscoveryClient : public NetworkDiscovery
    {
    protected:
        NetworkDiscoveryClient(int port, const std::string& service);
        ~NetworkDiscoveryClient();
        
        virtual void Update();
        virtual void OnMessage(ENetAddress sender, NetworkMessage& message);
        
    public:
        void Refresh();
        
        const std::vector<std::string>& GetHosts();
        
    protected:
        std::vector<std::string> _Hosts;
        
        bool _RefreshPending;
        std::string _Service;
        
        friend class NetworkManager;
    };
    
    class NetworkConnection
    {
    protected:
        NetworkConnection(ENetPeer* peer);
        ~NetworkConnection();
        
    public:
        void Close();
        
        void Send(NetworkMessage& message);
        
    private:
        ENetPeer* _Peer;
        
        friend class NetworkHost;
        friend class NetworkManager;
    };
    
    class NetworkManager
    {
    public:
        NetworkManager();
        ~NetworkManager();
        
        static NetworkManager* Instance();
        
        std::string GetHostName();
        
        NetworkServer* StartServer(int port, int maxConnections);
        NetworkClient* ClientConnect(const std::string& address, int port);
        
        void CloseHost(NetworkHost* host);
        
        NetworkDiscoveryServer* StartDiscoveryServer(int port);
        NetworkDiscoveryClient* ClientDiscover(int port, const std::string& service);
        
        void CloseDiscovery(NetworkDiscovery* discovery);

        void Update();

    private:
        std::set<NetworkHost*> _Hosts;
        std::set<NetworkDiscovery*> _Discovery;
        
        static NetworkManager* _Instance;
        
        friend class NetworkConnection;
    };
}
