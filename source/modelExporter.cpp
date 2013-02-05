#include "pixelboost/graphics/renderer/model/model.h"

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

bool ModelExporter::Save(const std::string& filename) const
{
    if (!_ModelLoader)
        return false;
    
    const pb::ModelDefinition* model = _ModelLoader->GetModel();
    
    model->Save(filename);
    
    return false;
}

bool ModelExporter::Process()
{
    if (!_ModelLoader)
        return false;
    
    if (!_ModelLoader->Process())
        return false;
    
    _ModelLoader->GetModel()->CalculateBounds();
    
    return true;    
}
