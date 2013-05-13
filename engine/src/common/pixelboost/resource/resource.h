#pragma once

#include <string>

#include "pixelboost/framework/definitions.h"
#include "pixelboost/resource/definitions.h"
#include "pixelboost/resource/resourceManager.h"

#define PB_DECLARE_RESOURCE public: friend class pb::ResourcePool; friend class pb::ResourceManager; virtual pb::Uid GetResourceType() const; static pb::Uid GetStaticResourceType();
#define PB_DEFINE_RESOURCE(className) pb::Uid className::GetResourceType() const { return GetStaticResourceType(); } pb::Uid className::GetStaticResourceType() { return pb::TypeHash(#className); }

namespace pb
{
    
    class ResourcePool;
    
    class Resource
    {
    protected:
        Resource(ResourcePool* pool, const std::string& filename);
        virtual ~Resource();
        
    public:
        virtual Uid GetResourceType() const = 0;
        
        const std::string& GetFilename();
        
        void Load();
        void Unload();
        
        ResourceState GetState();
        
        ResourceError GetError();
        const std::string& GetErrorDetails();
        
    public:
        sigslot::Signal2<Resource*, bool> SignalResourceLoaded;
        sigslot::Signal1<Resource*> SignalResourceUnloading;
        
    protected:
        virtual ResourceReadyState IsReadyToProcess(ResourceProcess process, std::string& errorDetails);
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails) = 0;
        virtual ResourceThread GetResourceThread(ResourceProcess process);
        
    private:
        ResourceReadyState IsReadyToProcess();
        void Process();
        ResourceThread GetThread(ResourceProcess state);
        
        static ResourceProcess GetProcessForState(ResourceState state);
        void SetState(ResourceState state);
        
        void NotifyStateChange();
        
    private:
        std::mutex _ProcessingMutex;
        
        ResourcePool* _Pool;
        std::string _Filename;
        
        ResourceState _State;
        ResourceError _Error;
        std::string _ErrorDetails;
        
        friend class ResourceManager;
        friend class ResourcePool;
    };

}
