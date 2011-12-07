#pragma once

#define NETWORK_MAX_MESSAGE_LENGTH 65535

#include <sys/types.h>

namespace pixelboost
{

class NetworkMessage
{
public:
    NetworkMessage();
    NetworkMessage(const NetworkMessage& src);
    ~NetworkMessage();
    
    bool ReadChar(char& value);
    bool ReadByte(__uint8_t& value);
    bool ReadInt(__int32_t& value);
    bool ReadFloat(float& value);
    bool ReadString(const char*& value);
    
    bool WriteChar(char value);
    bool WriteByte(__uint8_t value);
    bool WriteInt(__int32_t value);
    bool WriteFloat(float value);
    bool WriteString(const char* value);
    
    bool SetData(__int32_t length, char* data);
    
    void SetProtocol(__uint32_t protocol);
    __uint32_t GetProtocol();
    
    int GetDataLength();
    int GetMessageLength();
    
    int ConstructMessage(char* buffer, __int32_t maxLength);
    
private:
    bool HasRemaining(__int32_t length);
    
    __uint32_t _Protocol;
    
    char* _Buffer;
    int _Offset;
    __int32_t _Length;
};

}
