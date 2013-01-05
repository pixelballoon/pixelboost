#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "lodepng/lodepng.h"

#include "pixelboost/data/json/writer.h"

#include "modelExporter.h"
#include "modelLoader.h"

int main (int argc, const char * argv[])
{
    std::string inputLocation = argc > 1 ? argv[1] : "";
    
    std::string outputLocation = argc > 2 ? argv[2] : "";
    
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
        printf("ERROR: No loader available for extension %s on model %s\n", extension.c_str(), inputLocation.c_str());
        return 1;
    }
    
    if (!modelExporter->Process())
    {
        printf("ERROR: Failed to process model %s\n", inputLocation.c_str());
        return 1;
    }
    
    modelExporter->Save(outputLocation);
    
    printf("Saving model %s -> %s\n", inputLocation.c_str(), outputLocation.c_str());

    return 0;
}
