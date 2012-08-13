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
        
        bool Load(pb::FileLocation location, const std::string& directory);
        
        std::wstring GetString(pb::Uid key, const std::string& str);
        
    private:
        void ParseString(std::string& str);
        void ParseString(std::wstring& str);
        
        typedef std::map<pb::Uid, std::wstring> StringMap;
        StringMap _Strings;
    };
    
    std::string GetCurrentLocale();
    
}

#define _(key) pb::Localisation::Instance()->GetString(pb::TypeHash(key), key)