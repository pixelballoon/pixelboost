#include "pixelboost/debug/assert.h"
#include "pixelboost/debug/log.h"
#include "pixelboost/resource/resourceManager.h"
#include "pixelboost/resource/resource.h"

using namespace pb;

ResourceHandleBase::ResourceHandleBase(ResourcePool* pool, const std::string& filename, Resource* resource)
    : _Pool(pool)
    , _Filename(filename)
    , _State(kResourceStateLoading)
    , _Error(kResourceErrorNone)
    , _Resource(resource)
{
    PbAssert(resource && "Resource is null, has the resource type been registered with the ResourceManager?");
}

ResourceHandleBase::~ResourceHandleBase()
{
    ResourceManager::Instance()->AddDeletedResource(_Resource);
}

void ResourceHandleBase::Process()
{
    if (_Resource->ProcessResource(_State, _Filename, _ErrorDetails))
    {
        switch (_State)
        {
            case kResourceStateLoading:
                _State = kResourceStateProcessing;
                break;
            case kResourceStateProcessing:
                _State = kResourceStatePostProcessing;
                break;
            case kResourceStatePostProcessing:
                _State = kResourceStateComplete;
                break;
            case kResourceStateComplete:
            case kResourceStateError:
                PbAssert(!"Processing a resource in this state should never occur");
                break;
            case kResourceStateUnloading:
                break;
        }
    } else {
        _State = kResourceStateError;
    }
}

void ResourceHandleBase::Unload()
{
    _Pool->UnloadResource(_Filename);
}

ResourceThread ResourceHandleBase::GetThread(pb::ResourceState state)
{
    return _Resource->GetResourceThread(state);
}

ResourceState ResourceHandleBase::GetState()
{
    return _State;
}

ResourceError ResourceHandleBase::GetError()
{
    return _Error;
}

std::string ResourceHandleBase::GetErrorDetails()
{
    return _ErrorDetails;
}

ResourcePool::ResourcePool()
    : _Priority(0)
{
    
}

ResourcePool::~ResourcePool()
{
    
}

int ResourcePool::GetPriority()
{
    return _Priority;
}

void ResourcePool::SetPriority(int priority)
{
    _Priority = priority;
}

void ResourcePool::UnloadResource(const std::string& filename)
{
    auto resource = _Resources.find(filename);
    
    if (resource != _Resources.end())
    {
        _Resources.erase(resource);
    }
}

void ResourcePool::UnloadAllResources()
{
    _Resources.clear();
}

bool ResourcePool::HasPending()
{
    return _Pending.size();
}

std::shared_ptr<ResourceHandleBase> ResourcePool::GetPending(ResourceState resourceState)
{
    if (!_Pending.size())
        return 0;
    
    auto it = _Pending.begin();
    do
    {
        ResourceState state = (*it)->GetState();
        
        if (state == kResourceStateComplete || state == kResourceStateError)
        {
            it = _Pending.erase(it);
        } else if (state == resourceState)
        {
            break;
        } else {
            ++it;
        }
    } while (it != _Pending.end());
    
    if (it != _Pending.end())
        return *it;
    
    return 0;
}

ResourceManager* ResourceManager::Instance()
{
    static ResourceManager* instance = new ResourceManager();
    
    return instance;
}

ResourceManager::ResourceManager()
    : _IsLoading(false)
    , _IsProcessing(false)
    , _IsPostProcessing(false)
{
    
}

ResourceManager::~ResourceManager()
{
    
}

void ResourceManager::Update(float timeDelta)
{
    Purge();
    Process(kResourceStateLoading, _IsLoading, _LoadingThread);
    Process(kResourceStateProcessing, _IsProcessing, _ProcessingThread);
    Process(kResourceStatePostProcessing, _IsPostProcessing, _PostProcessingThread);
}

ResourcePool* ResourceManager::GetPool(const std::string& name)
{
    auto pool = _Pools.find(name);
    
    if (pool != _Pools.end())
        return pool->second;
    
    return 0;
}

ResourcePool* ResourceManager::CreatePool(const std::string& name)
{
    if (_Pools.find(name) != _Pools.end())
    {
        PbLogWarn("pb.resource.pool", "Trying to create pool %s that already exists, returning existing pool", name.c_str());
        return _Pools.find(name)->second;
    }

    ResourcePool* pool = new ResourcePool();
    
    _Pools[name] = pool;
    
    return pool;
}

void ResourceManager::DestroyPool(const std::string& name)
{
    auto pool = _Pools.find(name);
    
    if (pool == _Pools.end())
    {
        PbLogWarn("pb.resource.pool", "Unable to destroy pool %s as it doesn't exist", name.c_str());
        return;
    }
    
    delete pool->second;
    _Pools.erase(pool);
}

void ResourceManager::RegisterResourceCreator(Uid type, ResourceCreator resourceCreator)
{
    _ResourceCreation[type] = resourceCreator;
}

void ResourceManager::Process(ResourceState state, bool& handleVariable, std::thread& thread)
{
    if (!handleVariable)
    {
        if (thread.joinable())
            thread.join();
        
        std::shared_ptr<ResourceHandleBase> resource = 0;
        
        for (auto it = _Pools.begin(); it != _Pools.end(); ++it)
        {
            resource = it->second->GetPending(state);
            
            if (resource)
                break;
        }
        
        if (resource && resource->_ProcessingMutex.try_lock())
        {
            handleVariable = true;
            
            if (resource->GetThread(state) == kResourceThreadMain)
            {
                ProcessResource(resource, handleVariable);
            } else {
                thread = std::thread(&ResourceManager::ProcessResource, this, resource, std::ref(handleVariable));
            }
        }
    }
}

void ResourceManager::ProcessResource(std::shared_ptr<ResourceHandleBase> resource, bool& handleVariable)
{
    resource->Process();
    handleVariable = false;
    resource->_ProcessingMutex.unlock();
}

void ResourceManager::AddDeletedResource(Resource* resource)
{
    _DeletedResources.push_back(resource);
}

void ResourceManager::Purge()
{
    for (const auto& resource : _DeletedResources)
    {
        PurgeResource(resource);
    }
    _DeletedResources.clear();
}

void ResourceManager::PurgeResource(Resource *resource)
{
    std::string error;
    resource->ProcessResource(kResourceStateUnloading, "", error);
    delete resource;
}

Resource* ResourceManager::CreateResourceData(int typeId)
{
    auto resourceCreator = _ResourceCreation.find(typeId);
    
    if (resourceCreator == _ResourceCreation.end())
        return 0;
    
    return resourceCreator->second();
}
