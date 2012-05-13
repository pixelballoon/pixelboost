#pragma once

#include <string>
#include <vector>

class mg_connection;
class mg_context;

namespace pb
{
    class HttpConnection
    {
    public:
        HttpConnection(mg_connection* connection);
        ~HttpConnection();
        
        void AddHeader(const std::string& headerName, const std::string& headerValue);
        void SetContent(const std::string& data);
        
        void Send();
        
    private:
        std::string _Data;
        mg_connection* _Connection;
    };
    
    class HttpServer
    {
    public:
        enum RequestType
        {
            kRequestTypeGet,
            kRequestTypePut,
            kRequestTypePost,
            kRequestTypeDelete,
        };
        
    public:
        HttpServer();
        ~HttpServer();
        
        void Start(int port, const std::string& documentRoot = "");
        void Stop();
        
        void Update();
        
        virtual bool OnHttpRequest(RequestType type, const std::string& uri, const std::string& query, const std::string& data, HttpConnection& connection);
        
    private:
        mg_context* _Context;
    };
}
