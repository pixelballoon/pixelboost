#pragma once

#define NETWORK_MAX_MESSAGE_LENGTH 262140

#include <stdint.h>
#include <sys/types.h>

namespace pb
{

class NetworkMessage
{
public:
    NetworkMessage();
    NetworkMessage(const NetworkMessage& src);
    ~NetworkMessage();
    
    bool ReadChar(char& value);
    bool ReadByte(uint8_t& value);
    bool ReadInt(int32_t& value);
    bool ReadFloat(float& value);
    bool ReadString(const char*& value);
    
    bool WriteChar(char value);
    bool WriteByte(uint8_t value);
    bool WriteInt(int32_t value);
    bool WriteFloat(float value);
    bool WriteString(const char* value);
    
    bool SetData(int32_t length, char* data);
    
    void SetProtocol(uint32_t protocol);
    uint32_t GetProtocol();
    
    int GetDataLength();
    int GetMessageLength();
    
    int ConstructMessage(char* buffer, int32_t maxLength);
    
private:
    bool HasRemaining(int32_t length);
    
    uint32_t _Protocol;
    
    char* _Buffer;
    int _Offset;
    int32_t _Length;
};

}
