#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "libpixel/debug/debugVariable.h"
#include "libpixel/debug/debugVariableManager.h"
#include "libpixel/file/fileHelpers.h"
#include "libpixel/json/writer.h"
#include "libpixel/network/networkMessage.h"

namespace libpixel
{
    
DebugVariableManager* DebugVariableManager::_Instance = 0;

DebugVariableManager::DebugVariableManager(const std::string& htmlLocation)
    : HttpServer()
{
    Start(9091, FileHelpers::GetRootPath()+"/"+htmlLocation);
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

#ifndef PIXELBALLOON_BUILD_RELEASE

void DebugVariableManager::AddVariable(DebugVariable* variable)
{
    _Variables[variable->GetName()] = variable;
}
    
const DebugVariableManager::VariableMap& DebugVariableManager::GetVariables() const
{
    return _Variables;
}

bool DebugVariableManager::OnHttpRequest(HttpServer::RequestType type, const std::string& uri, const std::string& query, const std::string& data, HttpConnection& connection)
{
    ssize_t split = uri.find('/', 1);
    
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
        ssize_t split = argumentString.find('/', 1);
        
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
    
    if (command == "getVariables" && type == kRequestTypeGet)
    {
        OnGetVariables(connection);
        return true;
    } else if (command == "setVariable" && type == kRequestTypePut)
    {
        if (arguments.size() && data != "")
        {
            DebugVariableManager::VariableMap::const_iterator it = _Variables.find(arguments[0]);
            
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
        variable["name"] = json::String(it->first);
        
        std::string type;
        
        switch (it->second->GetVariableType())
        {
            case DebugVariable::kVariableTypeBool:
            {
                DebugBool* debugVariable = static_cast<DebugBool*>(it->second);
                variable["type"] = json::String("bool");
                variable["value"] = json::Boolean(debugVariable->_Value);
                break;
            }
            case DebugVariable::kVariableTypeString:
            {
                DebugString* debugVariable = static_cast<DebugString*>(it->second);
                variable["type"] = json::String("string");
                variable["value"] = json::String(debugVariable->_Value);
                break;
            }
            case DebugVariable::kVariableTypeInteger:
            {
                DebugInteger* debugVariable = static_cast<DebugInteger*>(it->second);
                variable["type"] = json::String("int");
                variable["min"] = json::Number(debugVariable->_Min);
                variable["max"] = json::Number(debugVariable->_Max);
                variable["value"] = json::Number(debugVariable->_Value);
                break;
            }
            case DebugVariable::kVariableTypeFloat:
            {
                DebugFloat* debugVariable = static_cast<DebugFloat*>(it->second);
                variable["type"] = json::String("float");
                variable["min"] = json::Number(debugVariable->_Min);
                variable["max"] = json::Number(debugVariable->_Max);
                variable["value"] = json::Number(debugVariable->_Value);
                break;
            }
            case DebugVariable::kVariableTypeColor:
            {
                DebugColor* debugVariable = static_cast<DebugColor*>(it->second);
                variable["type"] = json::String("color");
                json::Object value;
                value["r"] = json::Number(debugVariable->_R);
                value["g"] = json::Number(debugVariable->_G);
                value["b"] = json::Number(debugVariable->_B);
                value["a"] = json::Number(debugVariable->_A);
                variable["value"] = value;
                break;
            }
            case DebugVariable::kVariableTypeFunction:
            {
                variable["type"] = json::String("function");
                break;
            }
        }
        
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
            static_cast<DebugString*>(variable)->SetValue(static_cast<json::String>(params["value"]).Value().c_str());
            break;
        }
            
        case DebugVariable::kVariableTypeBool:
        {
            static_cast<DebugBool*>(variable)->_Value = static_cast<json::Boolean>(params["value"]).Value();;
            break;
        }
            
        case DebugVariable::kVariableTypeColor:
        {
            DebugColor* colour = static_cast<DebugColor*>(variable);
            colour->_R = static_cast<json::Number>(params["value"]["r"]).Value();
            colour->_G = static_cast<json::Number>(params["value"]["g"]).Value();
            colour->_B = static_cast<json::Number>(params["value"]["b"]).Value();
            colour->_A = static_cast<json::Number>(params["value"]["a"]).Value();
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
    
#endif

}
