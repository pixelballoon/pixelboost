#include "pixelboost/graphics/shader/shader.h"
#include "pixelboost/graphics/shader/manager.h"

using namespace pb;

ShaderManager::ShaderManager()
{
    
}

ShaderManager::~ShaderManager()
{
    
}

Shader* ShaderManager::LoadShader(const std::string& file, bool reload)
{
    ShaderMap::iterator it = _Shaders.find(file);
    
    if (it != _Shaders.end())
    {
        if (reload)
        {
            it->second->Load(file);
        }
        
        return it->second;
    }
    
    Shader* shader = new Shader();
    shader->Load(file);
    _Shaders[file] = shader;
    return shader;
}

void ShaderManager::UnloadShader(const std::string& file)
{
    ShaderMap::iterator it = _Shaders.find(file);
    
    if (it != _Shaders.end())
    {
        delete it->second;
        _Shaders.erase(it);
    }
}

Shader* ShaderManager::GetShader(const std::string& file)
{
    ShaderMap::iterator it = _Shaders.find(file);
    
    if (it != _Shaders.end())
        return it->second;
    
    return 0;
}
