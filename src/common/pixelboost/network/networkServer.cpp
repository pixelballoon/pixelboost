#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>

#include "pixelboost/network/networkMessage.h"
#include "pixelboost/network/networkServer.h"

namespace pb
{
    
    NetworkServer* NetworkServer::_Instance = 0;
    
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
    
    NetworkConnection::NetworkConnection(NetworkServer* networkManager)
    : _NetworkManager(networkManager)
    , _IsOpen(false)
    , _Connection(0)
    , _IsReading(false)
    , _SendLength(0)
    , _RecvLength(0)
    , _SendOffset(0)
    , _RecvOffset(0)
    {
        _SendBuffer = new char[NETWORK_MAX_MESSAGE_LENGTH];
        _RecvBuffer = new char[NETWORK_MAX_MESSAGE_LENGTH];
    }
    
    NetworkConnection::~NetworkConnection()
    {
        delete[] _SendBuffer;
        delete[] _RecvBuffer;
    }
    
    void NetworkConnection::Close()
    {
        if (_IsOpen)
        {
            close(_Connection);
            
            _IsOpen = false;
            
            _NetworkManager->ConnectionClosed(*this);
            
            _RecvOffset = 0;
            _SendOffset = 0;
            _SendLength = 0;
            _RecvLength = 0;
            _IsReading = false;
        }
    }
    
    bool NetworkConnection::IsOpen()
    {
        return _IsOpen;
    }
    
    int NetworkConnection::GetConnectionId()
    {
        return _Connection;
    }
    
    void NetworkConnection::Update()
    {
        ProcessSend();
        ProcessRecv();
    }
    
    void NetworkConnection::Send(NetworkMessage& message)
    {
        if (!_IsOpen)
            return;
        
        _SendQueue.push(new NetworkMessage(message));
    }
    
    void NetworkConnection::ProcessSend()
    {
        int sendProcessed = 0;
        
        while (sendProcessed < 50)
        {
            if (_SendLength == 0)
            {
                if (_SendQueue.size())
                {
                    NetworkMessage* message = _SendQueue.front();
                    _SendQueue.pop();
                    int messageLength = message->ConstructMessage(_SendBuffer, NETWORK_MAX_MESSAGE_LENGTH);
                    delete message;
                    _SendLength = messageLength;
                    _SendOffset = 0;
                }
                else
                {
                    return;
                }
            }
            
            if (_SendLength)
            {
                int sendLength = static_cast<int>(send(_Connection, &_SendBuffer[_SendOffset], _SendLength, 0));
                
                if (sendLength <= 0)
                    return;
                
                _SendLength -= sendLength;
                _SendOffset += sendLength;
            }
            
            sendProcessed++;
        }
    }
    
    void NetworkConnection::ProcessRecv()
    {
        int recvProcessed = 0;
        
        while (recvProcessed < 50)
        {
            int maxRecv = _IsReading ? _RecvLength-_RecvOffset : 4-_RecvOffset;
            
            if (maxRecv == 0)
            {
                NetworkMessage msg;
                msg.SetData(_RecvLength, &_RecvBuffer[4]);
                
                _NetworkManager->HandleMessage(*this, msg);
                
                _RecvOffset = 0;
                _IsReading = false;
                maxRecv = 4;
            }
            
            int recvLength = static_cast<int>(recv(_Connection, &_RecvBuffer[_RecvOffset], maxRecv, 0));
            
            if (recvLength == 0)
            {
                _IsOpen = false;
                _NetworkManager->ConnectionClosed(*this);
                return;
            }
            
            if (recvLength < 0)
            {
                if (errno == EAGAIN)
                    return;
                
                if (errno == ECONNRESET)
                {
                    _IsOpen = false;
                    _NetworkManager->ConnectionClosed(*this);
                    return;
                }
                
                fprintf(stderr, "Error Receiving: %s\n", strerror(errno));
                
                return;
            }
            
            if (!_IsReading && (_RecvOffset+recvLength) == 4)
            {
                __int32_t messageLength;
                memcpy(&messageLength, _RecvBuffer, 4);
                _RecvLength = ntohl(messageLength);
                _IsReading = true;
            }
            
            _RecvOffset += recvLength;
            
            recvProcessed++;
        }
    }
    
    
    void NetworkConnection::SetConnection(int connection)
    {
        _IsOpen = true;
        _Connection = connection;
        _IsReading = false;
        _RecvLength = 0;
        _SendLength = 0;
        _RecvOffset = 0;
        _SendOffset = 0;
        
        _NetworkManager->ConnectionOpened(*this);
    }
    
    NetworkServer::NetworkServer()
    : _MaxConnections(0)
    , _ServerState(kStateDisconnected)
    , _ClientState(kStateDisconnected)
    {
        _ClientConnection = new NetworkConnection(this);
        _ClientHost = new char[255];
        _Instance = this;
    }
    
    NetworkServer::~NetworkServer()
    {
        _Instance = 0;
        
        _Handlers.clear();
        
        StopServer();
        CloseClient();
        
        delete[] _ClientHost;    
        delete _ClientConnection;
    }
    
    NetworkServer* NetworkServer::Instance()
    {
        return _Instance;
    }
    
    void NetworkServer::StartServer(int port, int maxConnections)
    {
        StopServer();
        
        _MaxConnections = maxConnections;
        _ServerPort = port;
        _ServerState = kStateStarting;
        
        for (int i=0; i<maxConnections; i++)
            _ServerConnections.push_back(new NetworkConnection(this));
    }
    
    void NetworkServer::StopServer()
    {
        if (_ServerState == kStateDisconnected)
            return;
        
        _ServerState = kStateDisconnecting;
        
        while (_ServerState != kStateDisconnected)
            Update();
        
        for (ConnectionList::iterator it = _ServerConnections.begin(); it != _ServerConnections.end(); ++it)
            delete *it;    
        _ServerConnections.clear();
    }
    
    void NetworkServer::OpenClient(const char* host, int port)
    {
        CloseClient();
        
        strncpy(_ClientHost, host, 255);
        _ClientPort = port;
        _ClientState = kStateStarting;
    }
    
    void NetworkServer::CloseClient()
    {
        if (_ClientState == kStateDisconnected)
            return;
        
        _ClientState = kStateDisconnecting;
        
        while (_ClientState != kStateDisconnected)
            Update();
    }
    
    NetworkConnection& NetworkServer::GetClientConnection()
    {
        return *_ClientConnection;
    }
    
    void NetworkServer::Update()
    {
        switch (_ServerState)
        {
            case kStateStarting:
            {         
                _ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                
                int flags = fcntl(_ServerSocket, F_GETFL, 0);
                fcntl(_ServerSocket, F_SETFL, flags|O_NONBLOCK);
                
                int optval = 1;
                setsockopt(_ServerSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
                
                _ServerState = kStateBinding;
                
                break;
            }
                
            case kStateBinding:
            {
                sockaddr_in server;
                
                server.sin_family = AF_INET;
                server.sin_addr.s_addr = htonl(INADDR_ANY);
                server.sin_port = htons(_ServerPort);
                
                if (bind(_ServerSocket, (struct sockaddr*)&server, sizeof(sockaddr_in)) == 0)
                {
                    listen(_ServerSocket, _MaxConnections);
                    
                    _ServerState = kStateListening;
                } else {
                    fprintf(stderr, "Error Binding: %s\n", strerror(errno));
                }
                
                break;
            }
                
            case kStateListening:
            {           
                int connectionId = accept(_ServerSocket, 0, 0);
                
                for (ConnectionList::iterator it = _ServerConnections.begin(); it != _ServerConnections.end(); ++it)
                {
                    if ((*it)->IsOpen())
                    {
                        (*it)->Update();
                    }
                    else
                    {
                        if (connectionId > 0)
                        {
                            (*it)->SetConnection(connectionId);
                            connectionId = 0;
                        }
                    }
                }
                
                break;
            }
                
            case kStateDisconnecting:
            {
                for (ConnectionList::iterator it = _ServerConnections.begin(); it != _ServerConnections.end(); ++it)
                {
                    if ((*it)->IsOpen())
                        (*it)->Close();
                }
                close(_ServerSocket);
                _ServerState = kStateDisconnected;
                break;
            }
                
            default:
                break;
        }
        
        switch (_ClientState)
        {
            case kStateStarting:
            {         
                _ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                
                int flags = fcntl(_ClientSocket, F_GETFL, 0);
                fcntl(_ClientSocket, F_SETFL, flags|O_NONBLOCK);
                
                int optval = 1;
                setsockopt(_ClientSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
                
                _ClientState = kStateConnecting;
                
                break;
            }
                
            case kStateConnecting:
                sockaddr_in server;
                
                server.sin_family = AF_INET;
                inet_pton(AF_INET, _ClientHost, &(server.sin_addr));
                server.sin_port = htons(_ClientPort);
                
                if (connect(_ClientSocket, (struct sockaddr*)&server, sizeof(sockaddr_in)) == 0)
                {
                    _ClientState = kStateConnected;
                    _ClientConnection->SetConnection(_ClientSocket);
                } else {
                    if (errno == EISCONN)   
                    {
                        _ClientState = kStateConnected;
                        _ClientConnection->SetConnection(_ClientSocket);
                        break;
                    }
                    
                    if (errno != EINPROGRESS && errno != EALREADY)
                    {
                        close(_ClientSocket);
                        _ClientState = kStateStarting;
                    }
                }
                
                break;
                
            case kStateConnected:
            {
                if (_ClientConnection->IsOpen())
                    _ClientConnection->Update();
                else
                    _ClientState = kStateConnecting;
                
                break;
            }
                
            case kStateDisconnecting:
            {
                _ClientConnection->Close();
                _ClientState = kStateDisconnected;
                break;
            }
                
            default:
                break;
        }
    }
    
    bool NetworkServer::RegisterHandler(NetworkHandler* handler)
    {
        if (_Handlers.find(handler->GetProtocol()) != _Handlers.end())
            return false;
        
        _Handlers[handler->GetProtocol()] = handler;
        
        return true;
    }
    
    void NetworkServer::SendMessage(NetworkMessage& message)
    {
        for (ConnectionList::iterator it = _ServerConnections.begin(); it != _ServerConnections.end(); ++it)
        {
            if ((*it)->IsOpen())
            {
                (*it)->Send(message);
            }
        }
    }
    
    void NetworkServer::ConnectionOpened(NetworkConnection& connection)
    {
        for (HandlerMap::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
            it->second->OnConnectionOpened(connection);
    }
    
    void NetworkServer::ConnectionClosed(NetworkConnection& connection)
    {
        for (HandlerMap::iterator it = _Handlers.begin(); it != _Handlers.end(); ++it)
            it->second->OnConnectionClosed(connection);
    }
    
    void NetworkServer::HandleMessage(NetworkConnection& connection, NetworkMessage& message)
    {
        HandlerMap::iterator it = _Handlers.find(message.GetProtocol());
        
        if (it == _Handlers.end())
            return;
        
        it->second->OnReceive(connection, message);
    }
    
}
