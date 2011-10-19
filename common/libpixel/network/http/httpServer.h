#pragma once

#include <string>
#include <vector>

class mg_connection;
class mg_context;

namespace libpixel
{
    class HttpConnection
    {
    public:
        HttpConnection(mg_connection* connection);
        ~HttpConnection();
        
        void SetHeader(const std::string& contentType);
        void Write(const std::string& data);
        
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
        
        void Start(int port);
        void Stop();
        
        void Update();
        
        virtual void OnHttpRequest(RequestType type, const std::string& uri, const std::string& query, HttpConnection connection);
        
    private:
        mg_context* _Context;
    };
}
