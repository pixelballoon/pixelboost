#include <cstddef>
#include <cstring>

#include  "pixelboost/external/mongoose/mongoose.h"
#include  "pixelboost/network/http/httpServer.h"

using namespace pb;

static void* MongooseCallback(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *requestInfo)
{
    HttpServer* server = static_cast<HttpServer*>(requestInfo->user_data);
    
    switch (event)
    {
        case MG_NEW_REQUEST:
        {
            HttpServer::RequestType requestType;
            if (strcmp(requestInfo->request_method, "GET") == 0)
                requestType = HttpServer::kRequestTypeGet;
            else if (strcmp(requestInfo->request_method, "PUT") == 0)
                requestType = HttpServer::kRequestTypePut;
            else if (strcmp(requestInfo->request_method, "POST") == 0)
                requestType = HttpServer::kRequestTypePost;
            else if (strcmp(requestInfo->request_method, "DELETE") == 0)
                requestType = HttpServer::kRequestTypeDelete;
      
            char dataString[8192];
            int read = mg_read(conn, dataString, 8192);
            dataString[read] = 0;
            
            
            HttpConnection connection(conn);
            if (server->OnHttpRequest(requestType, requestInfo->uri, requestInfo->query_string ? requestInfo->query_string : "", dataString, connection))
                return (void*)1;
            
            break;
        }
            
        case MG_EVENT_LOG:
            printf("%s", requestInfo->log_message);
            break;
            
        // Don't handle these for now
        case MG_HTTP_ERROR: 
        case MG_INIT_SSL:
            break;
    }
    
    return 0;
}

pb::HttpConnection::HttpConnection(mg_connection* connection)
    : _Connection(connection)
{
    _Data = "HTTP/1.1 200 OK\r\n";
}

pb::HttpConnection::~HttpConnection()
{
    
}

void pb::HttpConnection::AddHeader(const std::string& headerName, const std::string& headerValue)
{
    _Data += headerName + ": " + headerValue + "\r\n";
}

void pb::HttpConnection::SetContent(const std::string& data)
{
    _Data += "\r\n";
    _Data += data;
}

void pb::HttpConnection::Send()
{
    mg_write(_Connection, _Data.c_str(), _Data.length()+1);
}

pb::HttpServer::HttpServer()
    : _Context(0)
{
    
}

pb::HttpServer::~HttpServer()
{

}

void pb::HttpServer::Start(int port, const std::string& documentRoot)
{
    if (_Context != 0)
        return;
    
    char portString[128]; // "80,443s"
    sprintf(portString, "%d", port);
    
    if (documentRoot == "")
    {
        const char *options[] = {
            "listening_ports", portString,
            0
        };
    
        _Context = mg_start(&MongooseCallback, this, options);
    } else {
        const char *options[] = {
            "listening_ports", portString,
            "document_root", documentRoot.c_str(),
            0
        };
        
        _Context = mg_start(&MongooseCallback, this, options);
    }
}
                        
void pb::HttpServer::Stop()
{
    mg_stop(_Context);
    _Context = 0;
}

void pb::HttpServer::Update()
{
    
}

bool pb::HttpServer::OnHttpRequest(RequestType type, const std::string& uri, const std::string& query, const std::string& data, HttpConnection& connection)
{
    return false;
}
