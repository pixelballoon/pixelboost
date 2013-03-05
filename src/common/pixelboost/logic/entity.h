#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/framework/definitions.h"
#include "pixelboost/logic/definitions.h"

struct lua_State;

#define PB_DECLARE_ENTITY public: virtual pb::Uid GetType() const; static pb::Uid GetStaticType(); private:
#define PB_DEFINE_ENTITY(className) pb::Uid className::GetType() const { return GetStaticType(); } pb::Uid className::GetStaticType() { return pb::TypeHash(#className); }

namespace pb
{
    
class Component;
class DbEntity;
class Message;
class Scene;

class Entity
{
public:
    Entity(Scene* scene, Entity* parent, DbEntity* creationEntity);
    virtual ~Entity();
    
    static void RegisterLuaClass(lua_State* state);
    
public:
    enum EntityState
    {
        kEntityCreated,
        kEntityDestroyed
    };
    
public:
    Scene* GetScene();
    Entity* GetParent();
    
    Uid GetCreationUid();
    Uid GetUid();
    
    const std::string& GetName();
    void SetName(const std::string& name);
    
    virtual Uid GetType() const;
    static pb::Uid GetStaticType();
    
    const DbEntity* GetCreationEntity() const;
    template<class T> const T* GetData() const;

    void Destroy();
    EntityState GetState();

public:
    typedef std::map<Uid, Component*> ComponentMap;
    
public:
    const std::set<Entity*>& GetChildren();
    Entity* FindChildByName(const std::string& name);
    
    template <class T>T* CreateComponent();
    void DestroyComponent(Component* component);
    void DestroyAllComponents();
    
    template <class T>T* GetComponent();
    ComponentMap GetComponents();

    void RegisterMessageHandler(MessageHandler handler);
    void UnregisterMessageHandler(MessageHandler handler);
    template <class T> void RegisterMessageHandler(MessageHandler handler);
    template <class T> void UnregisterMessageHandler(MessageHandler handler);

private:
    void AddChild(Entity* child);
    void RemoveChild(Entity* child);
    
    void HandleMessage(const Message& message);
    
    void Purge();
    void SyncMessageHandlers();
    
private:
    void HandleCreationEntityDestroyed();
    void HandleCreationEntityReloaded();
    
private:
    typedef sigslot::Signal1<const Message&> MessageSignal;
    
    typedef std::map<Uid, MessageSignal> MessageHandlers;
    
    std::set<Entity*> _Children;
    
    ComponentMap _Components;
    std::set<Component*> _PendingComponents;
    
    MessageHandlers _MessageHandlers;
    MessageSignal _GenericMessageHandlers;
    
    int _HandlingMessage;
    std::map<Uid, std::vector<MessageHandler> > _MessageHandlerDelayedAdd;
    std::map<Uid, std::vector<MessageHandler> > _MessageHandlerDelayedRemove;
    
    Scene* _Scene;
    Entity* _Parent;
    
    DbEntity* _CreationEntity;
    
    Uid _Uid;
    
    std::string _Name;
    
    EntityState _State;
    
    friend class Component;
    friend class Scene;
};
    
}

#include "pixelboost/logic/entity.inl"
