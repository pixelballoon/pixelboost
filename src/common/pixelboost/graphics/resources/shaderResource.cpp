#include "pixelboost/graphics/resources/shaderResource.h"

using namespace pb;

ShaderResource::ShaderResource()
{
    
}

ShaderResource::~ShaderResource()
{
    
}

bool ShaderResource::ProcessResource(ResourceState state, const std::string& filename, ResourceError& error, std::string& errorDetails)
{
    error = kResourceErrorSystemError;
    errorDetails = "Loader not yet implemented";
    return true;
}

ResourceThread ShaderResource::GetResourceThread(ResourceState state)
{
    return kResourceThreadAny;
}

Shader* ShaderResource::GetShader()
{
    return _Shader;
}
