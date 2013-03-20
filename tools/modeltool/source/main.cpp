#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "json/writer.h"
#include "lodepng/lodepng.h"

#include "pipeline/debug/log.h"

#include "modelExporter.h"
#include "modelLoader.h"

int main(int argc, const char * argv[])
{
    if (argc < 2)
        return 1;
    
    std::string inputLocation = argv[1];
    std::string outputLocation = argv[2];
    
    PlLogDebug("modeltool", "Converting model from %s to %s", inputLocation.c_str(), outputLocation.c_str());
    
    std::string extension = inputLocation.substr(inputLocation.find_last_of(".")+1);

    ModelExporter* modelExporter;
    
    if (extension == "fbx")
    {
        modelExporter = new ModelExporter(new FbxLoader(inputLocation));
    }
    else if (extension == "obj")
    {
        modelExporter = new ModelExporter(new ObjLoader(inputLocation));
    }
    else
    {
        PlLogError("modeltool", "No loader available for extension %s on model %s", extension.c_str(), inputLocation.c_str());
        return 1;
    }
    
    if (!modelExporter->Process())
    {
        PlLogError("modeltool", "Failed to process model %s", inputLocation.c_str());
        return 1;
    }
    
    modelExporter->Save(outputLocation);

    return 0;
}
