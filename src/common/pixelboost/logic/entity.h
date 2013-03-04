#pragma once

#include <map>
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
    Entity(Scene* scene, DbEntity* creationEntity);
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
    
    Uid GetCreationUid();
    
    Uid GetUid();
    
    virtual Uid GetType() const;
    static pb::Uid GetStaticType();
    
    const DbEntity* GetCreationEntity() const;
    template<class T> const T* GetData() const;

    void Destroy();
    EntityState GetState();

public:
    typedef std::vector<Component*> ComponentList;
    
public:
    Uid GenerateComponentId();
        
public:
    void DestroyComponent(Component* component);
    void DestroyAllComponents();
    
    Component* GetComponentById(Uid componentId);
    
    template <class T>T* GetComponentByType();
    template <class T> ComponentList GetComponentsByType();

    void RegisterMessageHandler(MessageHandler handler);
    void UnregisterMessageHandler(MessageHandler handler);
    template <class T> void RegisterMessageHandler(MessageHandler handler);
    template <class T> void UnregisterMessageHandler(MessageHandler handler);
    
    void HandleMessage(const Message& message);
    
    virtual void OnCreationEntityDestroyed();
    virtual void OnCreationEntityReloaded();
    
private:
    void SyncMessageHandlers();
    
    void AddComponent(Component* component);
    void PurgeComponents();
    
    void HandleCreationEntityDestroyed();
    void HandleCreationEntityReloaded();
    
    typedef sigslot::Signal1<const Message&> MessageSignal;
    
    typedef std::map<Uid, MessageSignal> MessageHandlers;
    
    ComponentList _Components;
    MessageHandlers _MessageHandlers;
    MessageSignal _GenericMessageHandlers;
    
    int _HandlingMessage;
    std::map<Uid, std::vector<MessageHandler> > _MessageHandlerDelayedAdd;
    std::map<Uid, std::vector<MessageHandler> > _MessageHandlerDelayedRemove;
    
    Scene* _Scene;
    
    DbEntity* _CreationEntity;
    Uid _CreationUid;
    
    Uid _Uid;
    
    Uid _NextFreeUid;
    
    EntityState _State;
    
    friend class Component;
    friend class Scene;
};
    
}

#include "pixelboost/logic/entity.inl"
