#if defined(PIXELBOOST_PLATFORM_IOS)
    #include <UIKit/UIKit.h>
#elif defined(PIXELBOOST_PLATFORM_OSX)
    #include <Foundation/Foundation.h>
#endif

#include "enet/enet.h"

#include "pixelboost/debug/log.h"
#include "pixelboost/network/networkMessage.h"
#include "pixelboost/network/networkServer.h"

using namespace pb;

NetworkManager* NetworkManager::_Instance = 0;

NetworkHandler::NetworkHandler(int protocol)
    : _Protocol(protocol)
{
    
}

NetworkHandler::~NetworkHandler()
{
    
}

int NetworkHandler::GetProtocol()
{
    return _Protocol;
}

void NetworkHandler::OnConnectionOpened(NetworkConnection& connection)
{
    
}

void NetworkHandler::OnConnectionClosed(NetworkConnection& connection)
{
    
}

void NetworkHandler::OnReceive(NetworkConnection& connection, NetworkMessage& message)
{
    
}

NetworkHost::NetworkHost()
    : _Host(0)
{
    
}

NetworkHost::~NetworkHost()
{
    enet_host_destroy(_Host);
}

void NetworkHost::Close()
{
    NetworkManager::Instance()->CloseHost(this);
}

bool NetworkHost::RegisterHandler(NetworkHandler* handler)
{
    if (_Handlers.find(handler->GetProtocol()) != _Handlers.end())
        return false;
    
    _Handlers[handler->GetProtocol()].push_back(handler);
    
    return true;
}

void NetworkHost::BroadcastMessage(NetworkMessage& message)
{
    for (auto& connection : _Connections)
    {
        connection->Send(message);
    }
}

void NetworkHost::Update()
{
    ENetEvent event;
    
    while (enet_host_service (_Host, &event, 0) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                NetworkConnection* connection = new NetworkConnection(event.peer);
                
                _Connections.push_back(connection);
                
                for (auto& handlerList : _Handlers)
                {
                    for (auto& handler : handlerList.second)
                    {
                        handler->OnConnectionOpened(*connection);
                    }
                }
                
                break;
            }
                
            case ENET_EVENT_TYPE_RECEIVE:
            {
                NetworkMessage message;
                message.SetData(event.packet->dataLength, reinterpret_cast<char*>(event.packet->data));
                
                enet_packet_destroy (event.packet);
                
                auto& handlerList = _Handlers[message.GetProtocol()];
                
                for (auto& connection : _Connections)
                {
                    if (event.peer == connection->_Peer)
                    {
                        for (auto& handler : handlerList)
                        {
                            handler->OnReceive(*connection, message);
                        }
                    }
                }
                
                break;
            }
                
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                for (auto it = _Connections.begin(); it != _Connections.end(); ++it)
                {
                    if (event.peer == (*it)->_Peer)
                    {
                        for (auto& handlerList : _Handlers)
                        {
                            for (auto& handler : handlerList.second)
                            {
                                handler->OnConnectionClosed(**it);
                            }
                        }
                        _Connections.erase(it);
                        delete *it;
                        break;
                    }
                }
                
                break;
            }
                
            default:
                break;
        }
    }
}

void NetworkHost::HandleMessage(NetworkMessage& message)
{
    
}

NetworkClient::NetworkClient(const std::string& client, int port)
    : NetworkHost()
{
    _Host = enet_host_create(0, 1, 2, 0, 0);
    
    if (!_Host)
    {
        PbLogError("pb.network", "Error creating enet client host");
        return;
    }
    
    ENetAddress address;
    ENetPeer *peer;
    
    enet_address_set_host(&address, client.c_str());
    address.port = port;

    peer = enet_host_connect(_Host, &address, 2, 0);
    if (!peer)
    {
        PbLogDebug("pb.network", "No available peers for new connection to %s:%d", client.c_str(), port);
    }
}

NetworkClient::~NetworkClient()
{
    
}

NetworkServer::NetworkServer(int port, int maxConnections)
    : NetworkHost()
{
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;
    
    _Host = enet_host_create(&address, maxConnections, 2, 0, 0);
    
    if (_Host == 0)
    {
        PbLogError("pb.network", "Error creating enet server host");
        return;
    }
}

NetworkServer::~NetworkServer()
{
    
}

NetworkDiscovery::NetworkDiscovery(int port)
    : _Port(port)
    , _Socket(0)
{
    
}

NetworkDiscovery::~NetworkDiscovery()
{
    
}

void NetworkDiscovery::Update()
{
    if (!_Socket)
        return;
    
    ENetAddress sender;
    ENetBuffer buffer;
    
    buffer.data = malloc(128);
    buffer.dataLength = 128;
    
    int length = enet_socket_receive(_Socket, &sender, &buffer, 1);
    if (length > 0)
    {
        pb::NetworkMessage message;
        message.SetData(length, reinterpret_cast<char*>(buffer.data));
        
        OnMessage(sender, message);
    }
    
    free(buffer.data);
}

int NetworkDiscovery::Send(ENetSocket* output, ENetAddress* address, NetworkMessage& message)
{
    ENetSocket socket;

    socket = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
    if(socket == ENET_SOCKET_NULL)
        return -1;
    
    enet_socket_set_option(socket, ENET_SOCKOPT_NONBLOCK, 1);

    if(address->host == ENET_HOST_BROADCAST)
        enet_socket_set_option(socket, ENET_SOCKOPT_BROADCAST, 1);

    char* data = new char[message.GetMessageLength()];
    int length = message.ConstructMessage(data, message.GetMessageLength());
    
    ENetBuffer buffer;
    buffer.data = data;
    buffer.dataLength = length;
    int error = enet_socket_send(socket, address, &buffer, 1);
    
    delete data;
    
    if (output)
    {
        *output = socket;
    } else {
        enet_socket_destroy(socket);
    }
    
    if (error <= 0)
    {
        return error;
    }

    return length;
}

NetworkDiscoveryServer::NetworkDiscoveryServer(int port)
    : NetworkDiscovery(port)
{
    
}

NetworkDiscoveryServer::~NetworkDiscoveryServer()
{
    
}

NetworkDiscoveryServer* NetworkDiscoveryServer::AddService(const std::string& service)
{
    _Services.insert(service);
    return this;
}

void NetworkDiscoveryServer::Update()
{
    NetworkDiscovery::Update();
    
    if (!_Socket)
    {
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = _Port;
        
        _Socket = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
        if(_Socket == ENET_SOCKET_NULL)
        {
            return;
        }
        
        enet_socket_set_option(_Socket, ENET_SOCKOPT_REUSEADDR, 1);
        
        if(enet_socket_bind(_Socket, &address) < 0)
        {
            enet_socket_destroy(_Socket);
            _Socket = 0;
            return;
        }
        
        enet_socket_set_option(_Socket, ENET_SOCKOPT_NONBLOCK, 1);
    }
}

void NetworkDiscoveryServer::OnMessage(ENetAddress sender, NetworkMessage& message)
{
    if (message.GetProtocol() == 'dscr')
    {
        PbLogDebug("pb.network", "Send reply to discovery");
        
        std::string hostname = NetworkManager::Instance()->GetHostName();
        
        NetworkMessage reply;
        reply.SetProtocol('dsca');
        reply.WriteString(hostname.c_str());
        
        reply.WriteInt(_Services.size());
        for (const auto& service : _Services)
        {
            reply.WriteString(service.c_str());
        }
        
        Send(0, &sender, reply);
    }
}

NetworkDiscoveryClient::NetworkDiscoveryClient(int port, const std::string& service)
    : NetworkDiscovery(port)
    , _RefreshPending(true)
    , _Service(service)

{
    
}

NetworkDiscoveryClient::~NetworkDiscoveryClient()
{
    
}

void NetworkDiscoveryClient::Update()
{
    NetworkDiscovery::Update();
    
    if (_RefreshPending)
        Refresh();
}

void NetworkDiscoveryClient::OnMessage(ENetAddress sender, NetworkMessage& message)
{
    if (message.GetProtocol() == 'dsca')
    {
        const char* hostname;
        message.ReadString(hostname);
        
        int numServices;
        message.ReadInt(numServices);
        
        for (int i=0; i<numServices; i++)
        {
            const char* service;
            message.ReadString(service);
            
            if (service == _Service)
            {
                char ip[128];
                enet_address_get_host_ip(&sender, ip, 128);
                _Hosts.push_back(ip);
                PbLogDebug("pb.network", "Received reply to service (%s) discovery (%s/%s)", _Service.c_str(), hostname, ip);
                
                return;
            }
        }
    }
}

void NetworkDiscoveryClient::Refresh()
{
    _RefreshPending = true;
    
    ENetAddress address;
    address.host = ENET_HOST_BROADCAST;
    address.port = _Port;
    
    pb::NetworkMessage message;
    message.SetProtocol('dscr');
    
    if (_Socket)
    {
        enet_socket_destroy(_Socket);
        _Socket = 0;
    }
    
    if (Send(&_Socket, &address, message))
    {
        PbLogDebug("pb.network", "Sent discovery message");
        _RefreshPending = false;
    }
}

const std::vector<std::string>& NetworkDiscoveryClient::GetHosts()
{
    return _Hosts;
}

NetworkConnection::NetworkConnection(ENetPeer* peer)
    : _Peer(peer)
{

}

NetworkConnection::~NetworkConnection()
{
    
}

void NetworkConnection::Close()
{
    enet_peer_disconnect(_Peer, 0);
}

void NetworkConnection::Send(NetworkMessage& message)
{
    char* buffer = new char[message.GetMessageLength()];
    int length = message.ConstructMessage(buffer, message.GetMessageLength());

    ENetPacket* packet = enet_packet_create(buffer, length, ENET_PACKET_FLAG_RELIABLE);
    
    enet_peer_send(_Peer, 0, packet);
}

NetworkManager::NetworkManager()
{
    _Instance = this;
    
    enet_initialize();
}

NetworkManager::~NetworkManager()
{
    _Instance = 0;
    
    for (auto& host : _Hosts)
    {
        CloseHost(host);
    }
    
    for (auto& discovery : _Discovery)
    {
        CloseDiscovery(discovery);
    }
       
    enet_deinitialize();
}

NetworkManager* NetworkManager::Instance()
{
    return _Instance;
}

std::string NetworkManager::GetHostName()
{
#if defined(PIXELBOOST_PLATFORM_OSX)
    return [[[NSHost currentHost] localizedName] UTF8String];
#elif defined(PIXELBOOST_PLATFORM_IOS)
    return [[[UIDevice currentDevice] name] UTF8String];
#endif
    return "Unknown";
}

NetworkServer* NetworkManager::StartServer(int port, int maxConnections)
{
    NetworkServer* server = new NetworkServer(port, maxConnections);
    
    _Hosts.insert(server);
    
    return server;
}

NetworkClient* NetworkManager::ClientConnect(const std::string& address, int port)
{
    NetworkClient* client = new NetworkClient(address, port);
    
    _Hosts.insert(client);
    
    return client;
}

void NetworkManager::CloseHost(NetworkHost* host)
{
    _Hosts.erase(host);
    
    delete host;
}

NetworkDiscoveryServer* NetworkManager::StartDiscoveryServer(int port)
{
    NetworkDiscoveryServer* discovery = new NetworkDiscoveryServer(port);
    
    _Discovery.insert(discovery);
    
    return discovery;
}

NetworkDiscoveryClient* NetworkManager::ClientDiscover(int port, const std::string& service)
{
    NetworkDiscoveryClient* discovery = new NetworkDiscoveryClient(port, service);
    
    _Discovery.insert(discovery);
    
    return discovery;
}



void NetworkManager::CloseDiscovery(NetworkDiscovery* discovery)
{
    _Discovery.erase(discovery);
    
    delete discovery;
}

void NetworkManager::Update()
{
    for (auto& host : _Hosts)
    {
        host->Update();
    }
    
    for (auto& discovery : _Discovery)
    {
        discovery->Update();
    }
}
