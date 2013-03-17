#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "pixelboost/framework/definitions.h"
#include "pixelboost/resource/definitions.h"
#include "pixelboost/resource/resource.h"

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
        virtual ResourceReadyState IsReadyToProcess();
        virtual void Process();
        virtual ResourceThread GetThread(ResourceProcess state);
        
        static ResourceProcess GetProcessForState(ResourceState state);

    public:
        void Load();
        void Unload();
        
        ResourceState GetState();
        
        ResourceError GetError();
        const std::string& GetErrorDetails();
        
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
        
        T* GetResource();
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
        
        // GetResource will get a resource, if it doesn't exist it will load it, as soon as that resource is no longer
        // used, it will be unloaded
        template <class T> std::shared_ptr<ResourceHandle<T> > GetResource(const std::string& filename);

        // Load/unload resource work purely on resources that already exist
        void LoadResource(const std::string& filename);
        void UnloadResource(const std::string& filename);
        
        // Caching resources overrides the default behaviour of resource destruction. If a resource is no longer used
        // then it will remain loaded until discarded
        template <class T> void CacheResource(const std::string& filename);
        void DiscardResource(const std::string& filename);
        void DiscardAllResources(const std::string& filename);
        
        bool HasPending();
        std::shared_ptr<ResourceHandleBase> GetPending(ResourceState resourceState);
        
    private:
        int _Priority;
        
        std::mutex _ResourceMutex;
        std::vector<std::shared_ptr<ResourceHandleBase> > _Pending;
        std::map<std::string, std::weak_ptr<ResourceHandleBase> > _Resources;
        
        std::map<std::string, std::shared_ptr<ResourceHandleBase> > _CachedResources;
        
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
        
        void AddDeletedResource(Resource* resource);
        void Purge();
        void PurgeResource(Resource* resource);
        
        Resource* CreateResourceData(int typeId);
        
        bool _IsLoading;
        bool _IsProcessing;
        bool _IsPostProcessing;
        bool _IsUnloading;
        std::thread _LoadingThread;
        std::thread _ProcessingThread;
        std::thread _PostProcessingThread;
        std::thread _UnloadingThread;
        
        std::map<std::string, ResourcePool*> _Pools;
        std::map<Uid, ResourceCreator> _ResourceCreation;
        std::vector<Resource*> _DeletedResources;
        
        friend class ResourceHandleBase;
        friend class ResourcePool;
    };

}

#include "pixelboost/resource/resourceManager.inl"
