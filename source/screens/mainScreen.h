#pragma once

#include "pixelboost/framework/screen.h"
#include "pixelboost/network/networkServer.h"

namespace pb
{
    class NetworkClient;
    class NetworkConnection;
    class NetworkMessage;
    class OrthographicCamera;
    class Scene;
    class Viewport;
}

class MainInputHandler;

class MainScreen : public pb::Screen, public pb::NetworkHandler
{
public:
    MainScreen();
    ~MainScreen();
    
    virtual void Update(float timeDelta, float gameDelta);
    
    virtual void SetActive(bool active);
    
    void RefreshHosts();
    
private:
    virtual void OnConnectionOpened(pb::NetworkConnection& connection);
    virtual void OnConnectionClosed(pb::NetworkConnection& connection);
    
    virtual void OnReceive(pb::NetworkConnection& connection, pb::NetworkMessage& message);
    
    void AddControls();
    
    pb::NetworkDiscoveryClient* _DiscoveryClient;
    pb::NetworkClient* _Client;
    
    pb::Camera* _Camera;
    pb::Scene* _Scene;
    pb::Viewport* _Viewport;
};
