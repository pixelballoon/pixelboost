#include "pixelboost/graphics/resources/shaderResource.h"
#include "pixelboost/graphics/shader/shader.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::ShaderResource)

ShaderResource::ShaderResource()
{
    _Shader = 0;
}

ShaderResource::~ShaderResource()
{
    
}

bool ShaderResource::ProcessResource(ResourceState state, const std::string& filename, ResourceError& error, std::string& errorDetails)
{
    switch (state)
    {
        case kResourceStateLoading:
            _Shader = new Shader();
            if (!_Shader->Load(filename))
            {
                delete _Shader;
                _Shader = 0;
                error = kResourceErrorUnknown;
            }
            return true;
            
        case kResourceStateUnloading:
            delete _Shader;
            _Shader = 0;
            return true;

        default:
            return true;
    }
    error = kResourceErrorSystemError;
    errorDetails = "Loader not yet implemented";
    return true;
}

ResourceThread ShaderResource::GetResourceThread(ResourceState state)
{
    return kResourceThreadMain;
}

Shader* ShaderResource::GetShader()
{
    return _Shader;
}
