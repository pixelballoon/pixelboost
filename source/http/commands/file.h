#pragma once

#include <map>
#include <string>
#include <vector>

namespace pixelboost
{
    class HttpConnection;
}

namespace pixeleditor
{
    class FileCommands
    {
    public:
        FileCommands();
        
    private:
        bool OnExport(pixelboost::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments);
        bool OnSave(pixelboost::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments);
    };
}