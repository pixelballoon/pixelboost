#pragma once

namespace pixelboost
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
        bool OnGetRecords(pixelboost::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments);
    };
}
