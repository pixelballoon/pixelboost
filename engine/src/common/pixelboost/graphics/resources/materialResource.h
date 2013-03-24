#pragma once

#include <map>
#include <string>

#include "pixelboost/graphics/device/device.h"
#include "pixelboost/resource/resourceManager.h"

namespace pb
{
    
    class Material;
    class ShaderResource;
    class TextureResource;
    
    class MaterialResource : public pb::Resource
    {
        PB_DECLARE_RESOURCE
        
    protected:
        MaterialResource(ResourcePool* pool, const std::string& filename);
        ~MaterialResource();
        
    public:
        virtual ResourceReadyState IsReadyToProcess(ResourceProcess process, std::string& errorDetails);
        virtual ResourceError ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails);
        virtual ResourceThread GetResourceThread(ResourceProcess process);
        
        Material* GetMaterial();
        
    private:
        void ParseMaterial(ResourcePool* pool);
        
        bool ParseBoolValue(const std::string& value);
        GraphicsDevice::Blend ParseBlendValue(const std::string& value);
        
        void OnResourceLoaded(Resource* resource, bool error);
        void OnResourceUnloading(Resource* resource);
        
        Material* _Material;
        std::string _ShaderFilename;
        std::shared_ptr<ShaderResource> _Shader;
        std::map<std::string, std::shared_ptr<TextureResource> > _Textures;
        
        std::string _FileContents;
    };
    
}
