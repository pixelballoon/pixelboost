#pragma once

#include <map>
#include <string>

#include "pixelboost/db/definitions.h"
#include "pixelboost/file/fileSystem.h"

namespace pb
{
    
    class Localisation
    {
    public:
        Localisation();
        ~Localisation();
        
        static Localisation* Instance();
        
        bool Load(const std::string& directory);
        
        std::string GetString(pb::Uid key, const std::string& str);
        
    private:
        void ParseString(std::string& str);
        
        typedef std::map<pb::Uid, std::string> StringMap;
        StringMap _Strings;
        
        bool _Loaded;
    };
    
    std::string GetCurrentLocale();
    
}

#define _(key) pb::Localisation::Instance()->GetString(pb::TypeHash(key), key)