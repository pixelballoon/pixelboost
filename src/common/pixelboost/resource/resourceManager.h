#pragma once

#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "pixelboost/framework/definitions.h"
#include "pixelboost/resource/definitions.h"

namespace pb
{

class Resource;
class ResourcePool;
    
class ResourceHandleBase
{
protected:
    ResourceHandleBase(ResourcePool* pool, const std::string& filename, Resource* resource);
    
public:
    ~ResourceHandleBase();
    
protected:
    virtual void Process() = 0;
    
    virtual ResourceThread GetThread(ResourceState state) = 0;

public:
    ResourceState GetState();
    
    ResourceError GetError();
    std::string GetErrorDetails();
    
protected:
    std::mutex _ProcessingMutex;
    
    ResourcePool* _Pool;
    std::string _Filename;
    
    ResourceState _State;
    ResourceError _Error;
    std::string _ErrorDetails;
    
    Resource* _Resource;
    
    friend class ResourceManager;
    friend class ResourcePool;
};

template <class T> class ResourceHandle : public ResourceHandleBase
{
public:
    ResourceHandle(ResourcePool* pool, const std::string& filename, Resource* data);
    ~ResourceHandle();
    
protected:
    virtual void Process();
    
    virtual ResourceThread GetThread(ResourceState state);
    
public:
    T* operator->();
    
private:
};
    
class ResourcePool
{
private:
    ResourcePool();
    ~ResourcePool();
    
public:
    int GetPriority();
    void SetPriority(int priority);
    
    template <class T> std::shared_ptr<ResourceHandle<T> > GetResource(const std::string& filename);
    void UnloadResource(const std::string& filename);
    
    bool HasPending();
    std::shared_ptr<ResourceHandleBase> GetPending(ResourceState resourceState);
    
private:
    int _Priority;
    
    std::vector<std::shared_ptr<ResourceHandleBase> > _Pending;
    std::map<std::string, std::shared_ptr<ResourceHandleBase> > _Resources;
    
    friend class ResourceBase;
    friend class ResourceManager;
};
    
class ResourceManager
{
public:
    static ResourceManager* Instance();
    
private:
    ResourceManager();
    ~ResourceManager();
    
public:
    void Update(float timeDelta);
    
    ResourcePool* GetPool(const std::string& name);
    ResourcePool* CreatePool(const std::string& name);
    void DestroyPool(const std::string& name);
    
public:
    typedef Resource*(*ResourceCreator)();
    void RegisterResourceCreator(Uid type, ResourceCreator resourceCreator);
    
private:
    void Process(ResourceState state, bool& handleVariable, std::thread& thread);
    void ProcessResource(std::shared_ptr<ResourceHandleBase> resource, bool& handleVariable);
    
    Resource* CreateResourceData(int typeId);
    
    bool _IsLoading;
    bool _IsProcessing;
    bool _IsPostProcessing;
    std::thread _LoadingThread;
    std::thread _ProcessingThread;
    std::thread _PostProcessingThread;
    
    std::map<std::string, ResourcePool*> _Pools;
    std::map<Uid, ResourceCreator> _ResourceCreation;
    
    friend class ResourceBase;
    friend class ResourcePool;
};

}

#include "pixelboost/resource/resourceManager.inl"
