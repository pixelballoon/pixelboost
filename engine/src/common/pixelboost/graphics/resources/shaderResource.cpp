#include "pixelboost/debug/log.h"
#include "pixelboost/file/fileSystem.h"
#include "pixelboost/graphics/device/program.h"
#include "pixelboost/graphics/resources/shaderResource.h"
#include "pixelboost/graphics/shader/shader.h"

using namespace pb;

PB_DEFINE_RESOURCE(pb::ShaderResource)

ShaderResource::ShaderResource(ResourcePool* pool, const std::string& filename)
    : Resource(pool, filename)
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
            auto file = FileSystem::Instance()->OpenFile(filename);
            
            std::string contents;
            
            if (!file)
            {
                return kResourceErrorNoSuchResource;
            }
            
            file->ReadAll(contents);
            
            if (!_Document.load(contents.c_str()))
            {
                errorDetails = "Xml parse error";
                return kResourceErrorSystemError;
            }

            return kResourceErrorNone;
        }
            
        case kResourceProcessProcess:
        {
            _Shader = new Shader();
            if (!ParseShader(errorDetails))
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

        case kResourceProcessPostProcess:
        {
            return kResourceErrorNone;
        }
    }
}

ResourceThread ShaderResource::GetResourceThread(ResourceProcess process)
{
    if (process == kResourceProcessProcess)
        return kResourceThreadMain;
    
    return kResourceThreadAny;
}

Shader* ShaderResource::GetShader()
{
    return _Shader;
}

bool ShaderResource::ParseShader(std::string& errorDetails)
{
    pugi::xml_node root = _Document.child("shader");
    
    if (root.empty())
    {
        errorDetails = "Root node is empty";
        return false;
    }
    
    //pugi::xml_node properties = root.child("properties");
    
    pugi::xml_node techniques = root.child("technique");
    
    while (!techniques.empty())
    {
        ShaderTechnique* technique = new ShaderTechnique(TypeHash(techniques.attribute("name").value()));
        
        bool status = true;
        
        pugi::xml_node pass = techniques.child("pass");
        while (!pass.empty())
        {
            ShaderPass* shaderPass = new ShaderPass();
            
            pugi::xml_node program = pass.find_child_by_attribute("program", "language", shaderPass->GetShaderProgram()->GetShaderLanguage().c_str());
            
            if (program.empty() || !shaderPass->GetShaderProgram()->SetSource(program.child_value()))
            {
                delete shaderPass;
                status = false;
            } else {
                technique->AddPass(shaderPass);
            }
            
            pass = pass.next_sibling("pass");
        }
        
        if (status)
        {
            _Shader->AddTechnique(technique);
        } else {
            delete technique;
        }
        
        techniques = techniques.next_sibling("technique");
    }
    
    if (_Shader->GetNumTechniques() == 0)
    {
        errorDetails = "Failed to load shader, no techniques were loaded (are they compatible with this card?)";
        return false;
    }
    
    return true;
}
