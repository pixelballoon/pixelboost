#include <cstdio>

#include "libpixel/debug/debugVariable.h"
#include "libpixel/debug/debugVariableManager.h"
#include "libpixel/network/networkMessage.h"

namespace libpixel
{
    
DebugVariableManager* DebugVariableManager::_Instance = 0;

DebugVariableManager::DebugVariableManager()
    : NetworkHandler('dbvs')
{
    
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
    
void DebugVariableManager::VariableChanged(DebugVariable* variable)
{
    SendValue(variable);
}
    
void DebugVariableManager::SendValue(DebugVariable* variable)
{
    NetworkServer* networkServer = NetworkServer::Instance();
    
    if (!networkServer)
        return;
    
    NetworkMessage message;
    message.SetProtocol('dbvr');
    message.WriteString("value");
    message.WriteString(variable->GetName());
    message.WriteInt(variable->GetVariableType());
    
    switch (variable->GetVariableType())
    {
        case DebugVariable::kVariableTypeInteger:
        {
            message.WriteInt(static_cast<DebugInteger*>(variable)->_Value);
            break;
        }
            
        case DebugVariable::kVariableTypeFloat:
        {
            message.WriteFloat(static_cast<DebugFloat*>(variable)->_Value);
            break;
        }
            
        case DebugVariable::kVariableTypeString:
        {
            message.WriteString(static_cast<DebugString*>(variable)->_Value);
            break;
        }
            
        case DebugVariable::kVariableTypeBool:
        {
            message.WriteChar(static_cast<DebugBool*>(variable)->_Value);
            break;
        }
            
        case DebugVariable::kVariableTypeColour:
        {
            DebugColour* colour = static_cast<DebugColour*>(variable);
            message.WriteFloat(colour->_R);
            message.WriteFloat(colour->_G);
            message.WriteFloat(colour->_B);
            message.WriteFloat(colour->_A);
            break;
        }
            
        default:
            break;
    }
    
    networkServer->SendMessage(message);
}
    
const DebugVariableManager::VariableMap& DebugVariableManager::GetVariables() const
{
    return _Variables;
}

void DebugVariableManager::OnReceive(NetworkConnection& connection, NetworkMessage& message)
{
    const char* command;
    if (!message.ReadString(command))
        return;
    
    if (strcmp(command, "list") == 0)
    {
        NetworkMessage reply;
        reply.SetProtocol('dbvr');
        reply.WriteString("varlist");
        reply.WriteInt(static_cast<int>(_Variables.size()));
        
        for (VariableMap::const_iterator it = _Variables.begin(); it != _Variables.end(); ++it)
        {
            reply.WriteString(it->second->GetName());
            reply.WriteInt(it->second->GetVariableType());
            
            switch(it->second->GetVariableType())
            {
                case DebugVariable::kVariableTypeInteger:
                    reply.WriteInt(static_cast<DebugInteger*>(it->second)->_Min);
                    reply.WriteInt(static_cast<DebugInteger*>(it->second)->_Max);
                    break;
                
                case DebugVariable::kVariableTypeFloat:
                    reply.WriteFloat(static_cast<DebugFloat*>(it->second)->_Min);
                    reply.WriteFloat(static_cast<DebugFloat*>(it->second)->_Max);
                    break;
                    
                default:
                    break;
            }
        }
        
        connection.Send(reply);
    }
    
    if (strcmp(command, "value") == 0)
    {
        const char* name;
        if (!message.ReadString(name))
            return;
        
        VariableMap::const_iterator it = _Variables.find(name);
        
        if (it == _Variables.end())
            return;
        
        DebugVariableManager::Instance()->VariableChanged(it->second);        
    }
    
    if (strcmp(command, "set") == 0)
    {
        const char* name;
        if (!message.ReadString(name))
            return;
        
        DebugVariableManager::VariableMap::const_iterator it = _Variables.find(name);
        
        if (it != _Variables.end())
        {
            DebugVariable* variable = it->second;
            
            switch (variable->GetVariableType())
            {
                case DebugVariable::kVariableTypeInteger:
                {
                    int value;
                    if (!message.ReadInt(value))
                        return;
                    
                    static_cast<DebugInteger*>(variable)->_Value = value;
                    break;
                }
                    
                case DebugVariable::kVariableTypeFloat:
                {
                    float value;
                    if (!message.ReadFloat(value))
                        return;
                    
                    static_cast<DebugFloat*>(variable)->_Value = value;
                    break;
                }
                    
                case DebugVariable::kVariableTypeString:
                {
                    const char* value;
                    if (!message.ReadString(value))
                        return;
                    
                    static_cast<DebugString*>(variable)->SetValue(value);
                    break;
                }
                    
                case DebugVariable::kVariableTypeBool:
                {
                    char value;
                    if (!message.ReadChar(value))
                        return;
                    
                    static_cast<DebugBool*>(variable)->_Value = value;
                    break;
                }
                    
                case DebugVariable::kVariableTypeColour:
                {
                    float r, g, b, a;
                    if (!message.ReadFloat(r) || !message.ReadFloat(g) || !message.ReadFloat(b) || !message.ReadFloat(a))
                        return;
                    
                    DebugColour* colour = static_cast<DebugColour*>(variable);
                    colour->_R = r;
                    colour->_G = g;
                    colour->_B = b;
                    colour->_A = a;
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
    }
}
    
#endif

}
