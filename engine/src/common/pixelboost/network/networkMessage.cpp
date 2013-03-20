#if defined(PIXELBOOST_PLATFORM_WINDOWS)
    #include <Winsock2.h>
#elif defined(PIXELBOOST_PLATFORM_NACL)
    #define PIXELBOOST_DISABLE_NETWORKING
#else
	#include <arpa/inet.h>
#endif

#include <cstring>

#include "pixelboost/network/networkMessage.h"

namespace pb
{

NetworkMessage::NetworkMessage()
    : _Length(0)
    , _Offset(0)
    , _Protocol(0)
    , _Buffer(0)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    _Buffer = new char[NETWORK_MAX_MESSAGE_LENGTH];
#endif
}

NetworkMessage::NetworkMessage(const NetworkMessage& src)
    : _Protocol(src._Protocol)
    , _Offset(src._Offset)
    , _Length(src._Length)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    _Buffer = new char[NETWORK_MAX_MESSAGE_LENGTH];
    memcpy(_Buffer, src._Buffer, NETWORK_MAX_MESSAGE_LENGTH);
#endif
}

NetworkMessage::~NetworkMessage()
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    delete[] _Buffer;
#endif
}

bool NetworkMessage::ReadChar(char &value)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if (!HasRemaining(1))
        return false;
    
    value = _Buffer[_Offset];
    _Offset += 1;
    
    return true;
#else
    return false;
#endif
}

bool NetworkMessage::ReadByte(uint8_t &value)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if (!HasRemaining(1))
        return false;
    
    value = _Buffer[_Offset];
    _Offset += 1;
    
    return true;
#else
    return false;
#endif
}

bool NetworkMessage::ReadInt(int32_t &value)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if (!HasRemaining(4))
        return false;
    
    uint32_t netValue;
    memcpy(&netValue, &_Buffer[_Offset], 4);
    value = ntohl(netValue);
    
    _Offset += 4;
    
    return true;
#else
    return false;
#endif
}

bool NetworkMessage::ReadFloat(float &value)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if (!HasRemaining(4))
        return false;
    
    uint32_t netValue;
    memcpy(&netValue, &_Buffer[_Offset], 4);
    uint32_t hostValue = ntohl(netValue);
    value = *reinterpret_cast<float*>(&hostValue);
    
    _Offset += 4;
    
    return true;
#else
    return false;
#endif
}

bool NetworkMessage::ReadString(const char*& value)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    int32_t length;
    
    if (!ReadInt(length))
        return false;
    
    if (!HasRemaining(length + 1))
        return false;
    
    value = &_Buffer[_Offset];
    
    _Offset = _Offset + length + 1;
    
    return true;
#else
    return false;
#endif
}

bool NetworkMessage::WriteChar(char value)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if (!HasRemaining(1))
        return false;
    
    _Buffer[_Offset] = value;
    _Offset += 1;
    _Length += 1;
    
    return true;
#else
    return false;
#endif
}

bool NetworkMessage::WriteByte(uint8_t value)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if (!HasRemaining(1))
        return false;
    
    _Buffer[_Offset] = value;
    _Offset += 1;
    _Length += 1;
    
    return true;
#else
    return false;
#endif
}

bool NetworkMessage::WriteInt(int32_t value)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if (!HasRemaining(4))
        return false;
    
    uint32_t netValue = htonl(value);
    
    memcpy(&_Buffer[_Offset], &netValue, 4);
    _Offset += 4;
    _Length += 4;
    
    return true;
#else
    return false;
#endif
}

bool NetworkMessage::WriteFloat(float value)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if (!HasRemaining(4))
        return false;
    
    uint32_t netValue = htonl(*reinterpret_cast<int *>(&value));
    
    memcpy(&_Buffer[_Offset], &netValue, 4);
    _Offset += 4;
    _Length += 4;
    
    return true;
#else
    return false;
#endif
}

bool NetworkMessage::WriteString(const char* value)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    int length = static_cast<int>(strlen(value));
    
    if (!HasRemaining(length + 5))
        return false;
    
    WriteInt(length);
    memcpy(&_Buffer[_Offset], value, length+1);
    _Offset = _Offset + length + 1;
    _Length = _Length + length + 1;
    
    return true;
#else
    return false;
#endif
}

bool NetworkMessage::SetData(int32_t length, char* data)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if (length < 4 || length > NETWORK_MAX_MESSAGE_LENGTH)
        return false;
    
    // Parse protocol data before adding data to buffer
    length -= 4;
    
    uint32_t protocol;
    memcpy(&protocol, data, 4);
    
    _Protocol = ntohl(protocol);
    
    _Offset = 0;
    _Length = length;
    
    if (length)
        memcpy(_Buffer, &data[4], length);
    
    return true;
#else
    return false;
#endif
}

void NetworkMessage::SetProtocol(uint32_t protocol)
{
    _Protocol = protocol;
}

uint32_t NetworkMessage::GetProtocol()
{
    return _Protocol;
}

int NetworkMessage::GetDataLength()
{
    return _Length;
}

int NetworkMessage::GetMessageLength()
{
    return _Length + 4; // 4 bytes for protocol
}

int NetworkMessage::ConstructMessage(char* buffer, int maxLength)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if (GetMessageLength() > maxLength)
        return 0;
    
    uint32_t protocol = htonl(_Protocol);
    
    memcpy(&buffer[0], &protocol, 4);
    memcpy(&buffer[4], _Buffer, _Length);
    
    return GetMessageLength();
#else
    return 0;
#endif
}

bool NetworkMessage::HasRemaining(int length)
{
#ifndef PIXELBOOST_DISABLE_NETWORKING
    if ((_Offset + length) < NETWORK_MAX_MESSAGE_LENGTH)
        return true;
#endif

    return false;
}

}
