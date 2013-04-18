#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/framework/definitions.h"
#include "pixelboost/logic/definitions.h"

struct lua_State;

#define PB_DECLARE_ENTITY public: static pb::Entity* Create(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity); virtual pb::Uid GetType() const; static pb::Uid GetStaticType(); private:
#define PB_DEFINE_ENTITY_ABSTRACT(className) pb::Uid className::GetType() const { return GetStaticType(); } pb::Uid className::GetStaticType() { return pb::TypeHash(#className); }
#define PB_DEFINE_ENTITY(className) pb::Entity* className::Create(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity) { return new className(scene, parent, creationEntity); } PB_DEFINE_ENTITY_ABSTRACT(className)

namespace pb
{
    
class Component;
class DbEntity;
class Message;
class Scene;

class Entity
{
protected:
    Entity(Scene* scene, Entity* parent, DbEntity* creationEntity);
    virtual ~Entity();
    
    static pb::Entity* Create(Scene* scene, Entity* parent, DbEntity* creationEntity);
    
public:
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
    template <class T> const T* GetData() const;

    void Destroy();
    EntityState GetState();
    
public:
    const std::set<Entity*>& GetChildren();
    Entity* FindChildByName(const std::string& name);
    
    template <class T>T* CreateComponent();
    void DestroyComponent(Component* component);
    void DestroyAllComponents();
    
    template <class T>T* GetComponent();
    std::vector<Component*> GetComponents();

    void RegisterMessageHandler(MessageHandler handler);
    void UnregisterMessageHandler(MessageHandler handler);
    template <class T> void RegisterMessageHandler(MessageHandler handler);
    template <class T> void UnregisterMessageHandler(MessageHandler handler);
    
    void SendMessage(const Message& message, bool sendToSelf = true, bool broadcastParents = false, bool broadcastChildren = false);

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
    
    std::map<Uid, Component*> _Components;
    std::set<Component*> _PendingComponents;
    
    MessageHandlers _MessageHandlers;
    
    int _HandlingMessage;
    std::map<Uid, std::set<MessageHandler> > _MessageHandlerDelayedAdd;
    std::map<Uid, std::set<MessageHandler> > _MessageHandlerDelayedRemove;
    
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
