#pragma once

#include <string>

#include "pixelboost/file/fileSystem.h"

namespace pb
{
    
    class Localisation
    {
    public:
        Localisation();
        ~Localisation();
        
        static Localisation* Instance();
        
        bool Load(pb::FileLocation location, const std::string& filename);
        
        std::wstring GetString(const std::string& key);
    };
    
}

#define _(key) pb::Localisation::Instance()->GetString(key)