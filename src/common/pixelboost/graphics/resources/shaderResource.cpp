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

ResourceError ShaderResource::ProcessResource(ResourcePool* pool, ResourceProcess process, const std::string& filename, std::string& errorDetails)
{
    switch (process)
    {
        case kResourceProcessLoad:
        {
            _Shader = new Shader();
            if (!_Shader->Load(filename))
            {
                delete _Shader;
                _Shader = 0;
                return kResourceErrorUnknown;
            }
            return kResourceErrorNone;
        }
            
        case kResourceProcessUnload:
        {
            delete _Shader;
            _Shader = 0;
            return kResourceErrorNone;
        }

        case kResourceProcessProcess:
        case kResourceProcessPostProcess:
            return kResourceErrorNone;
    }
}

ResourceThread ShaderResource::GetResourceThread(ResourceProcess process)
{
    return kResourceThreadMain;
}

Shader* ShaderResource::GetShader()
{
    return _Shader;
}
