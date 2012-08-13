//#include <wchar.h>

#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/misc/stringHelpers.h"
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

bool Localisation::Load(pb::FileLocation location, const std::string& directory)
{
    std::string filename = directory + GetCurrentLocale() + ".po";
    
    std::wstring contents = pb::StringToUnicode(pb::FileHelpers::FileToString(location, filename));
    
    std::vector<std::wstring> lines;
    pb::StringHelpers::SplitString(contents, L'\n', lines);
    
    bool parsingKey = true;
    std::string key;
    std::wstring value;
    
    for (std::vector<std::wstring>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        std::wstring line = *it;
        
        if (line.length() == 0)
            continue;
        
        if (line.at(0) == L'm')
        {
            if (line.substr(0, 5) == L"msgid")
            {
                if (key.length())
                {
                    ParseString(key);
                    ParseString(value);
                    if (value.length())
                        _Strings[pb::RuntimeTypeHash(key)] = value;
                }

                parsingKey = true;
                key = pb::UnicodeToString(line.substr(7, line.length()-8));
            } else {
                parsingKey = false;
                value = line.substr(8, line.length()-9);
            }
        } else if (line.at(0) == '#')
        {
            continue;
        } else if (line.at(0) == '\"')
        {
            if (parsingKey)
            {
                key.append(pb::UnicodeToString(line.substr(1, line.length()-2)));
            } else {
                value.append(line.substr(1, line.length()-2));
            }
        }
    }
    
    if (key.length())
    {
        ParseString(key);
        ParseString(value);
        if (value.length())
            _Strings[pb::RuntimeTypeHash(key)] = value;
    }
    
    return true;
}

void Localisation::ParseString(std::string& str)
{
    size_t pos;
    
    pos = 0;
    while((pos = str.find("\\n", pos)) != std::string::npos)
    {
        str.replace(pos, 2, "\n");
        pos += 1;
    }
    
    pos = 0;
    while((pos = str.find("\\t", pos)) != std::string::npos)
    {
        str.replace(pos, 2, "\t");
        pos += 1;
    }
}

void Localisation::ParseString(std::wstring& str)
{
    size_t pos;
    
    pos = 0;
    while((pos = str.find(L"\\n", pos)) != std::wstring::npos)
    {
        str.replace(pos, 2, L"\n");
        pos += 1;
    }
    
    pos = 0;
    while((pos = str.find(L"\\t", pos)) != std::wstring::npos)
    {
        str.replace(pos, 2, L"\t");
        pos += 1;
    }
}

std::wstring Localisation::GetString(pb::Uid uid, const std::string& str)
{
    StringMap::iterator it = _Strings.find(uid);
    if (it != _Strings.end())
        return it->second;
    
    return pb::StringToUnicode(str);
}
