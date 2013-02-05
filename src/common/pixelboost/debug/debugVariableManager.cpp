#ifdef PIXELBOOST_PLATFORM_WINDOWS
	#include <BaseTsd.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "pixelboost/data/json/writer.h"
#include "pixelboost/debug/debugVariable.h"
#include "pixelboost/debug/debugVariableManager.h"
#include "pixelboost/file/fileHelpers.h"

using namespace pb;
    
DebugVariableManager* DebugVariableManager::_Instance = 0;

DebugVariableManager::DebugVariableManager(const std::string& htmlLocation)
    : HttpServer()
{
    Start(9091, FileHelpers::GetBundlePath()+"/"+htmlLocation);
}

DebugVariableManager::~DebugVariableManager()
{
    _Instance = 0;
}

DebugVariableManager* DebugVariableManager::Instance()
{
    if (!_Instance)
    {
        _Instance = new DebugVariableManager();
    }
    
    return _Instance;
}

#ifndef PIXELBOOST_BUILD_RELEASE

void DebugVariableManager::AddVariable(DebugVariable* variable)
{
    _Variables[variable->GetId()] = variable;
}
    
const DebugVariableManager::VariableMap& DebugVariableManager::GetVariables() const
{
    return _Variables;
}

bool DebugVariableManager::OnHttpRequest(HttpServer::RequestType type, const std::string& uri, const std::string& query, const std::string& data, HttpConnection& connection)
{
    int split = uri.find('/', 1);
    
    std::string command;
    std::string argumentString;
    std::vector<std::string> arguments;
    
    if (split > 0)
    {
        command = uri.substr(1, split-1);
        argumentString = uri.substr(split);
    } else {
        command = uri.substr(1);
    }
    
    while (argumentString.length() > 1)
    {
        int split = argumentString.find('/', 1);
        
        std::string argument;
        
        if (split > 0)
        {
            argument = argumentString.substr(1, split-1);
            argumentString = argumentString.substr(split);
        } else {
            argument = argumentString.substr(1);
            argumentString = "";
        }
        
        if (argument.length() > 0)
            arguments.push_back(argument);
        else
            break;
    }
    
    if (command == "variables" && type == kRequestTypeGet)
    {
        OnGetVariables(connection);
        return true;
    } else if (command == "variable" && type == kRequestTypeGet)
    {
        if (arguments.size())
        {
            DebugVariableManager::VariableMap::const_iterator it = _Variables.find(atoi(arguments[0].c_str()));
            
            if (it != _Variables.end())
            {
                OnGetVariable(connection, it->second);
                return true;
            }
        }
    } else if (command == "variable" && type == kRequestTypePut)
    {
        if (arguments.size() && data != "")
        {
            DebugVariableManager::VariableMap::const_iterator it = _Variables.find(atoi(arguments[0].c_str()));
            
            if (it != _Variables.end())
            {
                json::Object o;
                if (json::Reader::Read(o, data))
                {
                    OnSetVariable(connection, it->second, o);
                }
                return true;
            }
        }
    } else if (command == "variable" && type == kRequestTypeDelete)
    {
        if (arguments.size())
        {
            DebugVariableManager::VariableMap::const_iterator it = _Variables.find(atoi(arguments[0].c_str()));
            
            if (it != _Variables.end())
            {
                OnResetVariable(connection, it->second);
                return true;
            }
        }
    }
    
    if (type == kRequestTypeGet)
        return false; // No over-ride, default to finding html
    else
        return true;
}
    
void DebugVariableManager::OnGetVariables(HttpConnection& connection)
{
    json::Array variables;
    
    for (VariableMap::iterator it = _Variables.begin(); it != _Variables.end(); ++it)
    {
        json::Object variable;
        
        PopulateVariable(it->second, variable);
        
        variables.Insert(variable);
    }
    
    std::stringstream contentStream;
    json::Writer::Write(variables, contentStream);
    
    std::string content = contentStream.str();
    
    connection.AddHeader("Content-Type", "application/json;charset=utf-8");
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    connection.Send();
}
    
void DebugVariableManager::OnGetVariable(HttpConnection& connection, DebugVariable* variable)
{
    json::Object json;
    
    PopulateVariable(variable, json);
    
    std::stringstream contentStream;
    json::Writer::Write(json, contentStream);
    
    std::string content = contentStream.str();
    
    connection.AddHeader("Content-Type", "application/json;charset=utf-8");
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    connection.Send();
}
    
void DebugVariableManager::OnSetVariable(HttpConnection& connection, DebugVariable* variable, json::Object& params)
{    
    switch (variable->GetVariableType())
    {
        case DebugVariable::kVariableTypeInteger:
        {
            static_cast<DebugInteger*>(variable)->_Value = static_cast<json::Number>(params["value"]).Value();
            break;
        }
            
        case DebugVariable::kVariableTypeFloat:
        {
            static_cast<DebugFloat*>(variable)->_Value = static_cast<json::Number>(params["value"]).Value();
            break;
        }
            
        case DebugVariable::kVariableTypeString:
        {
            static_cast<DebugString*>(variable)->_Value = static_cast<json::String>(params["value"]).Value();
            break;
        }
            
        case DebugVariable::kVariableTypeBool:
        {
            static_cast<DebugBool*>(variable)->_Value = static_cast<json::Boolean>(params["value"]).Value();;
            break;
        }
            
        case DebugVariable::kVariableTypeColor:
        {
            DebugColor* color = static_cast<DebugColor*>(variable);
            color->_ValueR = static_cast<json::Number>(params["value"]["r"]).Value();
            color->_ValueG = static_cast<json::Number>(params["value"]["g"]).Value();
            color->_ValueB = static_cast<json::Number>(params["value"]["b"]).Value();
            color->_ValueA = static_cast<json::Number>(params["value"]["a"]).Value();
            break;
        }
            
        case DebugVariable::kVariableTypeFunction:
        {
            DebugFunction* function = static_cast<DebugFunction*>(variable);
            function->_Callback(function->_UserData);
            break;
        }
            
        default:
            break;
    }
}
    
void DebugVariableManager::OnResetVariable(HttpConnection& connection, DebugVariable* variable)
{
    variable->Reset();
    
    json::Object json;
    
    PopulateVariable(variable, json);
    
    std::stringstream contentStream;
    json::Writer::Write(json, contentStream);
    
    std::string content = contentStream.str();
    
    connection.AddHeader("Content-Type", "application/json;charset=utf-8");
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    connection.Send();
}
    
void DebugVariableManager::PopulateVariable(DebugVariable* variable, json::Object& o)
{
    o["name"] = json::String(variable->GetName());
    o["id"] = json::Number(variable->GetId());
    
    std::string type;
    
    switch (variable->GetVariableType())
    {
        case DebugVariable::kVariableTypeBool:
        {
            DebugBool* debugVariable = static_cast<DebugBool*>(variable);
            o["type"] = json::String("bool");
            o["originalValue"] = json::Boolean(debugVariable->_Original);
            o["value"] = json::Boolean(debugVariable->_Value);
            break;
        }
        case DebugVariable::kVariableTypeString:
        {
            DebugString* debugVariable = static_cast<DebugString*>(variable);
            o["type"] = json::String("string");
            o["originalValue"] = json::String(debugVariable->_Original);
            o["value"] = json::String(debugVariable->_Value);
            break;
        }
        case DebugVariable::kVariableTypeInteger:
        {
            DebugInteger* debugVariable = static_cast<DebugInteger*>(variable);
            o["type"] = json::String("int");
            o["min"] = json::Number(debugVariable->_Min);
            o["max"] = json::Number(debugVariable->_Max);
            o["value"] = json::Number(debugVariable->_Value);
            break;
        }
        case DebugVariable::kVariableTypeFloat:
        {
            DebugFloat* debugVariable = static_cast<DebugFloat*>(variable);
            o["type"] = json::String("float");
            o["min"] = json::Number(debugVariable->_Min);
            o["max"] = json::Number(debugVariable->_Max);
            o["originalValue"] = json::Number(debugVariable->_Original);
            o["value"] = json::Number(debugVariable->_Value);
            break;
        }
        case DebugVariable::kVariableTypeColor:
        {
            DebugColor* debugVariable = static_cast<DebugColor*>(variable);
            o["type"] = json::String("color");
            json::Object original;
            original["r"] = json::Number(debugVariable->_OriginalR);
            original["g"] = json::Number(debugVariable->_OriginalG);
            original["b"] = json::Number(debugVariable->_OriginalB);
            original["a"] = json::Number(debugVariable->_OriginalA);
            o["originalValue"] = original;
            json::Object value;
            value["r"] = json::Number(debugVariable->_ValueR);
            value["g"] = json::Number(debugVariable->_ValueG);
            value["b"] = json::Number(debugVariable->_ValueB);
            value["a"] = json::Number(debugVariable->_ValueA);
            o["value"] = value;
            break;
        }
        case DebugVariable::kVariableTypeFunction:
        {
            o["type"] = json::String("function");
            break;
        }
    }
    
    o["modified"] = json::Boolean(variable->HasVariableChanged());
}
    
#endif
