#include <cstddef>
#include <cstring>

#include  "libpixel/external/mongoose/mongoose.h"
#include  "libpixel/network/http/httpServer.h"

using namespace libpixel;

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
            
            HttpConnection connection(conn);
            server->OnHttpRequest(requestType, requestInfo->uri, requestInfo->query_string, connection);
            
            break;
        }
            
        // Don't handle these for now
        case MG_HTTP_ERROR: 
        case MG_EVENT_LOG:
        case MG_INIT_SSL:
            break;
    }
    
    return 0;
}

libpixel::HttpConnection::HttpConnection(mg_connection* connection)
    : _Connection(connection)
{

}

libpixel::HttpConnection::~HttpConnection()
{
    
}

void libpixel::HttpConnection::SetHeader(const std::string& contentType)
{
    _Data += "HTTP/1.1 200 OK\n";
    _Data += "Content-Type: " + contentType + "\n";
}

void libpixel::HttpConnection::Write(const std::string& data)
{
    _Data += data;
}

void libpixel::HttpConnection::Send()
{
    mg_write(_Connection, _Data.c_str(), _Data.length()+1);
}

libpixel::HttpServer::HttpServer()
    : _Context(0)
{
    
}

libpixel::HttpServer::~HttpServer()
{

}

void libpixel::HttpServer::Start(int port)
{
    if (_Context != 0)
        return;
    
    const char *options[] = {
        "listening_ports", "80,443s",
        0
    };
    
    _Context = mg_start(&MongooseCallback, this, options);    
}
                        
void libpixel::HttpServer::Stop()
{
    mg_stop(_Context);
    _Context = 0;
}

void libpixel::HttpServer::Update()
{
    
}

void libpixel::HttpServer::OnHttpRequest(RequestType type, const std::string& uri, const std::string& query, HttpConnection connection)
{
    connection.SetHeader("text/json");
    connection.Write("{ \"type\" : \"hello\" }");
    connection.Send();
}
