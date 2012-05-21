#pragma once

#include <map>
#include <string>
#include <vector>

namespace pb
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
        bool OnExport(pb::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments);
        bool OnSave(pb::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments);
    };
}