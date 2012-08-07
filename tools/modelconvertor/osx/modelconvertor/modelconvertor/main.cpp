#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "lodepng/lodepng.h"

#include "pixelboost/data/json/writer.h"

#include "model.h"
#include "modelExporter.h"
#include "modelLoader.h"

int main (int argc, const char * argv[])
{
    const char* inputLocation = argc > 1 ? argv[1] : "";
    
    const char* outputLocation = argc > 2 ? argv[2] : "";
    
    ModelExporter modelExporter(new ObjLoader(inputLocation));
    if (!modelExporter.Process())
    {
        printf("Failed to process model %s\n", inputLocation);
        return 1;
    }
    
    modelExporter.Save(outputLocation);
    
    printf("Saving model %s -> %s\n", inputLocation, outputLocation);

    return 0;
}
