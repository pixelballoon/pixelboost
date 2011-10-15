#include <string>

#include "libpixel/network/networkMessage.h"

namespace libpixel
{

NetworkMessage::NetworkMessage()
: _Length(0)
, _Offset(0)
, _Protocol(0)
{
    _Buffer = new char[NETWORK_MAX_MESSAGE_LENGTH];
}

NetworkMessage::NetworkMessage(const NetworkMessage& src)
: _Protocol(src._Protocol)
, _Offset(src._Offset)
, _Length(src._Length)
{
    _Buffer = new char[NETWORK_MAX_MESSAGE_LENGTH];
    memcpy(_Buffer, src._Buffer, NETWORK_MAX_MESSAGE_LENGTH);
}

NetworkMessage::~NetworkMessage()
{
    delete[] _Buffer;
}

bool NetworkMessage::ReadChar(char &value)
{
    if (!HasRemaining(1))
        return false;
    
    value = _Buffer[_Offset];
    _Offset += 1;
    
    return true;
}

bool NetworkMessage::ReadByte(__uint8_t &value)
{
    if (!HasRemaining(1))
        return false;
    
    value = _Buffer[_Offset];
    _Offset += 1;
    
    return true;
}

bool NetworkMessage::ReadInt(__int32_t &value)
{
    if (!HasRemaining(4))
        return false;
    
    __uint32_t netValue;
    memcpy(&netValue, &_Buffer[_Offset], 4);
    value = ntohl(netValue);
    
    _Offset += 4;
    
    return true;
}

bool NetworkMessage::ReadFloat(float &value)
{
    if (!HasRemaining(4))
        return false;
    
    __uint32_t netValue;
    memcpy(&netValue, &_Buffer[_Offset], 4);
    __uint32_t hostValue = ntohl(netValue);
    value = *reinterpret_cast<float*>(&hostValue);
    
    _Offset += 4;
    
    return true;
}

bool NetworkMessage::ReadString(const char*& value)
{
    __int32_t length;
    
    if (!ReadInt(length))
        return false;
    
    if (!HasRemaining(length + 1))
        return false;
    
    value = &_Buffer[_Offset];
    
    _Offset = _Offset + length + 1;
    
    return true;
}

bool NetworkMessage::WriteChar(char value)
{
    if (!HasRemaining(1))
        return false;
    
    _Buffer[_Offset] = value;
    _Offset += 1;
    _Length += 1;
    
    return true;
}

bool NetworkMessage::WriteByte(__uint8_t value)
{
    if (!HasRemaining(1))
        return false;
    
    _Buffer[_Offset] = value;
    _Offset += 1;
    _Length += 1;
    
    return true;
}

bool NetworkMessage::WriteInt(__int32_t value)
{
    if (!HasRemaining(4))
        return false;
    
    __uint32_t netValue = htonl(value);
    
    memcpy(&_Buffer[_Offset], &netValue, 4);
    _Offset += 4;
    _Length += 4;
    
    return true;
}

bool NetworkMessage::WriteFloat(float value)
{
    if (!HasRemaining(4))
        return false;
    
    __uint32_t netValue = htonl(*reinterpret_cast<int *>(&value));
    
    memcpy(&_Buffer[_Offset], &netValue, 4);
    _Offset += 4;
    _Length += 4;
    
    return true;
}

bool NetworkMessage::WriteString(const char* value)
{
    int length = static_cast<int>(strlen(value));
    
    if (!HasRemaining(length + 5))
        return false;
    
    WriteInt(length);
    memcpy(&_Buffer[_Offset], value, length+1);
    _Offset = _Offset + length + 1;
    _Length = _Length + length + 1;
    
    return true;
}

bool NetworkMessage::SetData(__int32_t length, char* data)
{
    if (length < 4 || length > NETWORK_MAX_MESSAGE_LENGTH)
        return false;
    
    // Parse protocol data before adding data to buffer
    length -= 4;
    
    __uint32_t protocol;
    memcpy(&protocol, data, 4);
    
    _Protocol = ntohl(protocol);
    
    _Offset = 0;
    _Length = length;
    
    if (length)
        memcpy(_Buffer, &data[4], length);
    
    return true;
}

void NetworkMessage::SetProtocol(__uint32_t protocol)
{
    _Protocol = protocol;
}

__uint32_t NetworkMessage::GetProtocol()
{
    return _Protocol;
}

int NetworkMessage::GetDataLength()
{
    return _Length;
}

int NetworkMessage::GetMessageLength()
{
    return _Length + 8; // 4 bytes for length, 4 for protocol
}

int NetworkMessage::ConstructMessage(char* buffer, int maxLength)
{
    if (GetMessageLength() > maxLength)
        return 0;
    
    __int32_t messageLength = htonl(GetMessageLength());
    
    __uint32_t protocol = htonl(_Protocol);
    
    memcpy(buffer, &messageLength, 4);
    memcpy(&buffer[4], &protocol, 4);
    memcpy(&buffer[8], _Buffer, _Length);
    
    return GetMessageLength();
}

bool NetworkMessage::HasRemaining(int length)
{
    if ((_Offset + length) < NETWORK_MAX_MESSAGE_LENGTH)
        return true;
    
    return false;
}

}
