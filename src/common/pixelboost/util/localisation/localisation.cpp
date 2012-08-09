#include "pixelboost/util/localisation/localisation.h"
#include "pixelboost/util/localisation/string.h"

using namespace pb;

Localisation::Localisation()
{
    
}

Localisation::~Localisation()
{
    
}

Localisation* Localisation::Instance()
{
    static Localisation* instance = 0;
    
    if (!instance)
        instance = new Localisation();
    
    return instance;
}

bool Localisation::Load(pb::FileLocation location, const std::string& filename)
{
    return false;
}

std::wstring Localisation::GetString(const std::string& key)
{
    return pb::StringToUnicode(key);
}
