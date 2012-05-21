#pragma once

namespace pb
{
    class HttpConnection;
}

namespace pixeleditor
{
    class RecordCommands
    {
    public:
        RecordCommands();
        
    private:
        bool OnGetRecords(pb::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments);
    };
}
