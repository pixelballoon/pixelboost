#include "modelExporter.h"
#include "modelLoader.h"

ModelExporter::ModelExporter(ModelLoader* modelLoader)
    : _ModelLoader(modelLoader)
{
    
}

ModelExporter::~ModelExporter()
{
    delete _ModelLoader;
}

bool ModelExporter::Save(const std::string& filename)
{
    return false;
}

bool ModelExporter::Process()
{
    if (_ModelLoader)
        return _ModelLoader->Process();
    
    return false;
}
