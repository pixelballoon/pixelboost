#include "pixelboost/data/resources/xmlResource.h"
#include "pixelboost/graphics/resources/modelResource.h"
#include "pixelboost/resource/register.h"
#include "pixelboost/resource/resourceManager.h"

namespace pb
{

PB_RESOURCE_DECLARE(ModelResource)
PB_RESOURCE_DECLARE(XmlResource)

void RegisterResources()
{
    PB_RESOURCE_REGISTER(ModelResource)
    PB_RESOURCE_REGISTER(XmlResource)
}

}