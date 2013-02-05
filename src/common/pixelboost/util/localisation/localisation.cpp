#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/misc/stringHelpers.h"
#include "pixelboost/util/localisation/localisation.h"
#include "pixelboost/util/localisation/string.h"

using namespace pb;

Localisation::Localisation()
    : _Loaded(false)
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

bool Localisation::Load(const std::string& directory)
{
    std::string filename = directory + GetCurrentLocale() + ".po";
    
    pb::File* file = pb::FileSystem::Instance()->OpenFile(filename);
    
    std::string contents;
    
    if (file)
    {
        file->ReadAll(contents);
        delete file;
    }
    
    if (!contents.length())
    {
        return false;
    }
    
    _Loaded = true;
    
    std::vector<std::string> lines;
    pb::StringHelpers::SplitString(contents, L'\n', lines);
    
    bool parsingKey = true;
    std::string key;
    std::string value;
    
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        std::string line = *it;
        
        if (line.length() == 0)
            continue;
        
        if (line.at(0) == L'm')
        {
            if (line.substr(0, 5) == "msgid")
            {
                if (key.length())
                {
                    ParseString(key);
                    ParseString(value);
                    if (value.length())
                        _Strings[pb::RuntimeTypeHash(key)] = value;
                }

                parsingKey = true;
                key = line.substr(7, line.length()-8);
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
                key.append(line.substr(1, line.length()-2));
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

std::string Localisation::GetString(pb::Uid uid, const std::string& str)
{
    StringMap::iterator it = _Strings.find(uid);
    if (it != _Strings.end())
        return it->second;
    
#ifndef PIXELBOOST_BUILD_RELEASE
    if (_Loaded)
        return "*** MISSING (" + str + ") ***";
    else
        return str;
#else
    return str;
#endif
}
