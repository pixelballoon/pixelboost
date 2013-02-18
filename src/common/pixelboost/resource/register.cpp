#include "pixelboost/data/xml/resource.h"
#include "pixelboost/resource/register.h"
#include "pixelboost/resource/resourceManager.h"

namespace pb
{
    PB_RESOURCE_DECLARE(XmlResource)
    
    void RegisterResources()
    {
        PB_RESOURCE_REGISTER(XmlResource)
    }
}
