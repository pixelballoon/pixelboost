#pragma once

#include "glm/glm.hpp"

#include "pixelboost/logic/message.h"

namespace pb
{
    
    class SetColorMessage : public Message
    {
        PB_DECLARE_MESSAGE
        
    public:
        SetColorMessage(glm::vec4 color);
        
        glm::vec4 GetColor();
        
    private:
        glm::vec4 _Color;
    };
    
}
